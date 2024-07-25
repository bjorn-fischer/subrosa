# subrosa

**subrosa** is a simple exec wrapper that creates a new network name space,
sets up a loopback interface, and execs whatever is given on the
command line. Must be install suid root.

## Motivation

You want to run some software without network connectivity for
testing or whatever? Containers or even VMs are to bloated or are too
cumbersome to set up? Then **subrosa** might do the trick:

    [bf@βελλεροφων:~]$ subrosa bash
    [bf@βελλεροφων:~]$ ifconfig 
    lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
            inet 127.0.0.1  netmask 255.0.0.0
            inet6 ::1  prefixlen 128  scopeid 0x10<host>
            loop  txqueuelen 1000  (Local Loopback)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

    [bf@βελλεροφων:~]$ ping 193.0.0.164
    ping: connect: Network is unreachable
    [bf@βελλεροφων:~]$

You can run Firefox in real offline mode. Or your favorite GIS mapping
software.

## Compiling and Installing

Just compile ```subrosa.c``` with your favorite C compiler, no extra
libraries needed:

    gcc -O -Wall -pedantic -o subrosa subrosa.c

Install the executable in ```/opt/local/bin``` or ```/usr/local/bin``` or
whereever it suits you. Please keep in mind that **subrosa** needs some
privileges to setup the namespace, so either install the executable suid
root, which should be safe. Or apply the proper capabilities (probably
cap_sys_admin and cap_net_admin) if you prefer this approach.

## Etymology

_Sub rosa_ is a latin idiom. Translated literally, under the rose,
denoting deliberate secrecy or confidentiality.


