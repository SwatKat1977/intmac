# Integrated Test Managemement Suite (INTMAC)

Integrated management suite for managing test cases.

** This is still in pre-alpha **

## Developers

This section is for developers who wish to run, modify or test INTAC.

### Prerequisites

For Docker:
* Docker installed.
* pip-tools installed if modifying the docker images.

### Running Docker Images Locally

Docker files have been provided to allow you to run INTMAC in Docker containers, the
following table details how to run and build each service locally.

Notes:
* To build a docker image you use: `docker image build <build parameters>`.
* To start the docker container you use: `docker run <run parameters>`.
* In the run parameters `-d` specifies running the container in the background.
* In the run parameters `-p` specifies running the port the container exposes.


Service | Tag | Build Parameters | Run Parameters
------------ | ------------- | ------------- | -------------
gateway-api | gateway-api | --file Dockerfile.gateway-api . | -d -p 2020:2020 gateway-api
