# tsduck-arib

> TSDuck with ARIB extensions for Japanese TV broadcast contents

![Linux](https://github.com/masnagam/tsduck-arib/workflows/Linux/badge.svg)
![macOS](https://github.com/masnagam/tsduck-arib/workflows/macOS/badge.svg)

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
$ build/install-prerequisites.sh
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

tsduck-arib is tested on Linux and macOS using GitHub Actions.

tsduck-arib can be built successfully on environments above.  But `make test`
fails in several tests frequently due to insufficient time precision.

The tests assume a time error less than 50ms.  But it's impossible to guarantee
the maximum wait time of threads/processes in a container running on a CI
service, especially in the macOS build.

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

[Jasmine]: https://jasmine.github.io/api/edge/Clock.html
[korfuri/fake_clock]: https://github.com/korfuri/fake_clock
