#!/bin/bash

TELEGRAM_BOT_TOKEN=6185028352:AAGMk3wUV5iVSrfzCtZfXT83NlDGFvEUqT8
TELEGRAM_USER_ID=579950685
TIME=60

if [[ "$CI_JOB_STATUS" == "success" ]]
then
  STATUS="$(echo "$1" ✅)"
else
  STATUS="$(echo "$1" ❌)"
fi

URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="Deploy status: $STATUS%0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"

curl -s --max-time $TIME -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null