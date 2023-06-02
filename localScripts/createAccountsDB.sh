cd items
export PYTHONPATH=`pwd`/shared:`pwd`/accounts_svc
export QUART_APP=accounts_svc
export QUART_DEBUG=true
export ITEMS_ACCOUNTS_SVC_CONFIG_FILE=../configurations/accounts_svc.config
export ITEMS_ACCOUNTS_SVC_CONFIG_FILE_REQUIRED=1
export BACKEND_INTERNAL_DB_FILENAME=../configurations/items_accounts_svc_new.db
export BACKEND_CREATE_DB_IF_MISSING=YES
uvicorn accounts_svc:app --host 0.0.0.0 --port 5050