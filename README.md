## Description

The **erlidna** library provides a simple interface to the [IDNA part of GNU libidn library](http://www.gnu.org/software/libidn/reference/libidn-idna.html) implemented in NIFs.

## Building and Installing

[![Build Status](https://travis-ci.org/thekvs/erlidna.svg?branch=master)](https://travis-ci.org/thekvs/erlidna)

The **erlidna** app is built with [rebar](https://github.com/basho/rebar), which must be in the command `PATH`.

## API

**erlidna** has a very simple API. You use erlidna:encode/1 to encode
internationalized domain names in ASCII strings using
[Punycode](http://en.wikipedia.org/wiki/Punycode) transcription and 
erlidna:decode/1 to decode back to UTF-8.

## Erlang Version

The **erlidna** app requires Erlang R14B or later.
