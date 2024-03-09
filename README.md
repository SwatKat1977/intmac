# Integrated Test Managemement Suite (ITEMS)

Integrated management suite for managing test cases.

** This is still in pre-alpha **

## Developers

This section is for developers who wish to run, modify or test ITEMS.

# Dependencies

To build INTMAC you need the following dependencies:

Key:
* ALL = All services
* AC = Accounts Service
* CM = CMS Service
* GW = Gateway Service

Dependency | Version | Applicable To
------------ | ------------- | -------------
[OatPP Framework](https://oatpp.io/) | LATEST | ALL
[Inja HTML Template Engine](https://github.com/pantor/inja) | LATEST | GW 

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

#### Accounts Service

Build:
docker build --file docker/Dockerfile.accounts_svc . -t items_accounts_svc

Run:
docker run -d --name items_accounts_svc --net items_net -p 5050:5000 --volume={CONFIG WITH FULL PATH}:/usr/local/items/accounts_svc.config items_accounts_svc  

Service | Tag | Build Parameters | Run Parameters
------------ | ------------- | ------------- | -------------
auth | items_auth | --file Dockerfile.auth . -t items_accounts_svc | -d -p 3030:3030 <Tag>
cms | items_cms | --file Dockerfile.cms . -t items_cms | -d -p 2222:2222 <Tag>
gateway-api | items_gateway_api | --file Dockerfile.gateway_svc . -t items_gateway_api | -d -p 2111:2111 <Tag>
web portal | items_web_portal | --file Dockerfile.web_portal . -t items_web_portal | -d -p 2333:2333 <Tag>
redis | items_redis | --file Dockerfile.redis . -t items_redis | -d -p 6379:6379 <Tag>

Example of building Accounts Service: docker image build --file Dockerfile.auth . -t items_auth

Example of running Auth: docker run -d -p 2020:2020 items_auth
