/*
 * (c) 2024 Björn Fischer,
 *          CeBiTec/BRF, Bielefeld University
 *
 * Simple exec wrapper that creates a new network name space,
 * sets up loopback interface, and execs whatever is given on the
 * command line. Must be install suid.
 *
 * Should compile with "gcc -O -Wall -pedantic -o subrosa subrosa.c".
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#define _GNU_SOURCE

#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct ifreq ifr;
    const char *if_name = "lo";
    int fd;
    char *msg;

    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <executable> [args] ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * detach from network namespace
     */
    msg = "Could not detach from network namespace";
    if (unshare(CLONE_NEWNET)) goto fail;

    /*
     * ip link lo set up
     */
    msg = "Could not setup loopback interface";
    if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) goto fail;
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) goto fail;
                                              // ifr_name after
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ); // SIOCGIFFLAGS is unspecified
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
    if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) goto fail;
    close(fd);

    /*
     * drop root privileges if we are suid root
     */
    if (geteuid() == 0 && getuid() != 0) {
        msg = "Could not drop privileges";
        if (setuid(getuid())) goto fail;
    }

    execvp(argv[1], &argv[1]);
    msg = "Could not exec";

fail:
    perror(msg);
    exit(EXIT_FAILURE);

}

