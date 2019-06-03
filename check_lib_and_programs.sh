#
# check if you have all installed
#
#
found_all=1
packages="gcc make libnl-dev libnl1 libssl-dev libiw-dev curl libcurl3 wireless-tools"
for p in $packages; do
    # echo "dpkg -p $p | grep \"Package: $p\""
    check=`dpkg -p $p 2> /dev/null | grep "Package: $p" | wc -l`
    if [ "$check" == "0" ]; then
        echo "Package $p is missing"
        found_all=0
    fi
done

if [ "$found_all" == "1" ]; then
    echo "All packages installed"
fi