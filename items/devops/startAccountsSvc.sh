export PYTHONPATH=`pwd`/shared:`pwd`/accounts_svc
export QUART_APP=accounts_svc
export QUART_DEBUG=true
export ITEMS_ACCOUNTS_SVC_CONFIG_FILE=configurations/accounts_svc.config
export ITEMS_ACCOUNTS_SVC_CONFIG_FILE_REQUIRED=1
quart run -p 5000