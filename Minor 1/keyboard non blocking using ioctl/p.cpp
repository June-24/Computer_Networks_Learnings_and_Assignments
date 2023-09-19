#include <iostream>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main() {
    int fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error opening file");
        return 1; // Return an error code
    }

    if (ioctl(fd, EVIOCGRAB, 1) == -1) {
        perror("Failed to grab keyboard input");
        close(fd);
        return 1; // Return an error code
    }

    struct input_event event;
    while (1) {

        ssize_t bytes = read(fd, &event, sizeof(struct input_event));
        if (bytes == sizeof(struct input_event)) {
            string s;
            cin>>s;
            cout<<"the string entered is "<<s<<endl;
            if (event.type == EV_KEY) {
                if (event.value == 1) {
                    cout << "Key Pressed" << endl;
                }
                else if (event.value == 0) {
                    cout << "Key Released" << endl;
                }
            }

            // Add a condition to break the loop if needed
            if (event.type == EV_KEY && event.code == KEY_ESC) {
                break; // Exit the loop when the Escape key is pressed
            }
        }else{
            cout<<"no key pressed"<<endl;
        }
        sleep(1);
    }

    close(fd);
    return 0;
}
