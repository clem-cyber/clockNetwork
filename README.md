# clockNetwork

To install clockNetwork do the following commands :
"wget https://raw.githubusercontent.com/clem-cyber/clockNetwork/master/install_clocknetwork"
"sudo chmod +x install_clocknetwork"

Then you can run the installation script :
"sudo ./install_clocknetwork"

Once the installation is complete, you have 2 commands :

"clocknetwork" -> to start the time server
NOTE : The server is automatically started on the 10013 port and on the ip address 127.0.0.1

"clock" -> to run the client 
NOTE : you have to give arguments to this command, the format is "clock {ip_address} {port_number}"

Once you have started the client you can run the following commands :
- "time" to know the time
NOTE : this command will give you the time but you can add "%H" for the hour and "%M" for the minutes to have your ideal format
- "exit" to quit the client program

To erase this application you just have to run the command : "clocknetwork.erase"
