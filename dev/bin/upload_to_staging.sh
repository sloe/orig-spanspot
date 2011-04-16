#!/bin/bash

TARGETDIR="/vol/www/spanspot_staging"
TARGETSVR="extserver0"
TARGETRSYNC="$TARGETSVR:$TARGETDIR"
TARGETPOSTSCRIPT="$TARGETDIR/dev/bin/post_update_remote.sh"

echo Uploading to spanspot_staging
# Check prerequisites
if [ "$(basename $(pwd))" != "spanspot" ] ; then
  echo This script must be run from the top level spanspot directory
  exit 1
fi

rsync -avz --exclude='.hg' $(pwd)/ "$TARGETRSYNC"

ssh extserver0 "cd $TARGETDIR && $TARGETPOSTSCRIPT"

echo Done.