export PYTHONPATH=`pwd`/shared:`pwd`/auth
export QUART_APP=auth
export QUART_DEBUG=true
export ITEMS_DATABASE_FILE=auth.db
export ITEMS_CREATE_WHEN_MISSING=1
quart run -p 3030