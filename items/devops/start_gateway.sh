export PYTHONPATH=`pwd`/shared:`pwd`/gateway_api
export QUART_APP=gateway_api
export QUART_DEBUG=true
export ITEMS_DATABASE_FILE=auth.db
export ITEMS_CREATE_WHEN_MISSING=1
quart run -p 3030