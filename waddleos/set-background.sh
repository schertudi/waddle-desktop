#!/bin/bash
#need to set DBUS_SESSION_BUS_ADDRESS for xfconf to work in cron job:
pid=$(ps -C xfce4-session -o pid=)
pid=$(echo $pid)
export $(grep -z DBUS_SESSION_BUS_ADDRESS /proc/$pid/environ)

props=$(xfconf-query -l -c xfce4-desktop | grep last-image)
#change background only if month no. of background doesn't match current month
firstProp=$(echo $props | awk '{print $1;}')

imagePath=$(xfconf-query -c xfce4-desktop -p $firstProp)

imageName=$(basename $imagePath)
imageName="${imageName%.*}" #remove extension

#only continue image if the current one fits required format
if [[ $imageName =~ (0[1-9]|1[0-2]) ]] #number from 01 to 12
then
  #echo "Background fits format, check if change is needed"
  currentMonth=$(date +%m)
  if [[ "$imageName" != "$currentMonth" ]]
  then
    echo "Month does not match, change background"
	echo $props | while read -r line; do
	xfconf-query --channel xfce4-desktop --property $line --set ~/.local/share/waddleos/backgrounds/$(date +%m).png
    done
  else
    echo "Month matches, don't touch background"
  fi
else
  echo "Current background name is not a number from 01 to 12, ignore"
fi


