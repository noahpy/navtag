
make

sed -i '/mfp=/d' commands.sh
echo "mfp=\"$(pwd)/marks.txt\"" >> commands.sh

echo "Creating link in /usr/bin"
sudo rm -f /usr/bin/navtag
sudo link navtag /usr/bin/navtag

echo
echo "------- IMPORTANT -------"
echo "Add the following line to your bashrc / zshrc:"
echo "source $(pwd)/commands.sh"
