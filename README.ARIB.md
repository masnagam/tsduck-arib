# tsduck-arib

> A TSDuck fork working with Japanese TV broadcast contents

![linux-build](https://img.shields.io/circleci/build/github/masnagam/tsduck-arib/master.svg?label=linux-build)
![Travis (.com) branch](https://img.shields.io/travis/com/masnagam/tsduck-arib/master.svg?label=macos-build)

## How to build with ARIB support

tsduck-arib uses aribb24 for ARIB STD-B24 character encoding support.  Install
it before building TSDuck:

```consone
$ mkdir -p /tmp/aribb24
$ curl -sSL https://github.com/masnagam/aribb24/archive/master.tar.gz | \
    tar -zx -C /tmp/aribb24 --strip-components=1
$ (cd /tmp/aribb24; ./bootstrap)
$ (cd /tmp/aribb24; ./configure --without-libpng)
$ (cd /tmp/aribb24; make && make install)
$ rm -rf /tmp/aribb24
```

The commands above installs under /usr/local.  Make sure that aribb24 can be
found in the pkg-config search path:

```console
$ pkg-config --libs aribb24
```

Set the `PKG_CONFIG_PATH` environment variable appropriately if the command
above fails.

Tools listed below are required for building aribb24:

* autoconf
* automake
* libtool

And then, build with `ARIB=1`:

```console
$ make ARIB=1
$ make install SYSPREFIX=/usr/local ARIB=1

# If you need header files for application development
$ make install-devel SYSPREFIX=/usr/local ARIB=1
```

tsduck-arib supports several build options.  See [building.md] for details.

[building.md]: src/doc/building.md

## Modifications

* Support ARIB STD-B24 character encoding
  * Converting Unicode characters into ARIB STD-B24 characters is not supported
* Support decoding ARIB-specific descriptors
  * Audio component descriptor
  * TS information descriptor

## Test environments

* [Circle CI] for Linux build
* [Travis CI] for macOS build

I'm planning to use [AppVeyor] for Wondows build.

tsduck-arib is built successfully on environments above.  But `make test` fail
in several tests frequently due to insufficient time precision.

The tests assume a time error less than 50ms.  But it's impossible to guarantee
the maximum wait time of threads/processes in a container running on a CI
service, especially on [Travis CI] for macOS build.

There are several solutions to fix the test failures:

* Disable test cases which fails only on CI services
  * utest has no option to disable specific test cases at this moment
* Relax the time precision requirement
* Use something like a fake clock which are commonly used in test frameworks
  like [Jasmine] in Node.js
  * This can be achieved in C/C++ by linking symbols such as `gettimeofday`
    with mock functions
  * I've not tried, but [korfuri/fake_clock] may be one of candidates

No solution has applied at this moment.

[Circle CI]: https://circleci.com/gh/masnagam/tsduck-arib
[Travis CI]: https://travis-ci.com/masnagam/tsduck-arib
[AppVeyor]: https://ci.appveyor.com/project/masnagam/tsduck-arib
[Jasmine]: https://jasmine.github.io/api/edge/Clock.html
[korfuri/fake_clock]: https://github.com/korfuri/fake_clock

### Alpine Linux

tsduck-arib can be built on Alpine Linux with a small modification like below:

```console
sed -i -e '1s/^#define _IOC(/#define _IOC_(/' /usr/include/bits/ioctl.h
sed -i -e '2i #define _IOC(a,b,c,d) ((int)_IOC_(a,b,c,d))' /usr/include/bits/i
```

But functions depending on the pthreads don't work due to an incompatibility
with the pthreads API on Alpine Linux.  The incompatibility causes an assertion
failure at [L184 in utestMessageQueue.cpp].

[L184 in utestMessageQueue.cpp]: ./src/utest/utestMessageQueue.cpp#L184
