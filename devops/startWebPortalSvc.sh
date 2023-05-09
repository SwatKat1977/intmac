cd items
export PYTHONPATH=`pwd`/shared:`pwd`/web_portal
export QUART_APP=web_portal
export QUART_DEBUG=true
export ITEMS_WEB_PORTAL_SVC_CONFIG_FILE=../configurations/web_portal_svc.config
export ITEMS_WEB_PORTAL_SVC_CONFIG_FILE_REQUIRED=1
quart run -p 7000