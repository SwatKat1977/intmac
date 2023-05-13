cd items
export PYTHONPATH=`pwd`/shared:`pwd`/gateway_api
export QUART_APP=gateway_api
export QUART_DEBUG=true
export ITEMS_GATEWAY_SVC_CONFIG_FILE=../configurations/gateway_svc.config
export ITEMS_GATEWAY_SVC_CONFIG_FILE_REQUIRED=1
export ITEMS_CREATE_WHEN_MISSING=0
python3 -m quart run -p 4050