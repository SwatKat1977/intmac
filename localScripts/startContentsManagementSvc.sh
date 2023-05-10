cd items
export PYTHONPATH=`pwd`/shared:`pwd`/cms_svc
export QUART_APP=cms_svc
export QUART_DEBUG=true
export ITEMS_CMS_SVC_CONFIG_FILE=../configurations/cms_svc.config
export ITEMS_CMS_SVC_CONFIG_FILE_REQUIRED=1
quart run -p 6000