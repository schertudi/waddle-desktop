#!/bin/bash
#need to set DBUS_SESSION_BUS_ADDRESS for xfconf to work in cron job:
pid=$(ps -C xfce4-session -o pid=)
pid=$(echo $pid)
export $(grep -z DBUS_SESSION_BUS_ADDRESS /proc/$pid/environ)

props=$(xfconf-query -l -c xfce4-desktop | grep last-image)
echo $props | while read -r line; do
    xfconf-query --channel xfce4-desktop --property $line --set ~/.local/share/waddleos/backgrounds/$(date +%m).png
done




