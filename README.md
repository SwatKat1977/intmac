# Integrated Test Managemement Suite (ITEMS)

Integrated management suite for managing test cases.

** This is still in pre-alpha **

## Developers

This section is for developers who wish to run, modify or test ITEMS.

### Prerequisites

For Docker:
* Docker installed.
* pip-tools installed if modifying the docker images.

### Running Docker Images Locally

Docker files have been provided to allow you to run ITEMS in Docker containers, the
following table details how to run and build each service locally.

Notes:
* To build a docker image you use: `docker image build <build parameters>`.
* To start the docker container you use: `docker run <run parameters>`.
* In the run parameters `-d` specifies running the container in the background.
* In the run parameters `-p` specifies running the port the container exposes.


Service | Tag | Build Parameters | Run Parameters
------------ | ------------- | ------------- | -------------
auth | items_auth | --file Dockerfile.auth . -t service_tag | -d -p 3030:3030 <Tag>
cms | items_cms | --file Dockerfile.cms . -t service_tag | -d -p 2222:2222 <Tag>
gateway-api | items_gateway-api | --file Dockerfile.gateway-api . -t service_tag | -d -p 2111:2111 <Tag>
web portal | items_web_portal | --file Dockerfile.web_portal . -t service_tag | -d -p 2333:2333 <Tag>

Example of building Auth: docker image build --file Dockerfile.auth . -t items_auth

Example of running Auth: docker run -d -p 2020:2020 items_auth
