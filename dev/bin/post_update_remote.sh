#!/bin/sh

echo Executing post-update script on $(hostname) in $(pwd)

for file in spanners config.php ; do
  if [ ! -e "$file" ] ; then
    target="../spanspot_dev/$file"
    if [ ! -e "$target" ] ; then
      echo "*** Link target '$target' does not exist"
      exit 1
    fi
    ln -s "$target" $file
  fi
done

# Replace Moyea payer with the generic version, as we're under the staging domain
rm modules/flashvideo/moyea/codebase.swf
ln -s codebase_generic.swf modules/flashvideo/moyea/codebase.swf
