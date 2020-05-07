import sys
from os.path import basename
from platformio import util

Import('env')

version = "1"

#
# Push new firmware to the Bintray storage using API
#


def save_firmware_for_ota(source, target, env):
    firmware_path = str(source[0])
    # firmware_name = basename(firmware_path)

    print(firmware_path)


# Custom upload command and program name
env.Replace(
    PROGNAME="firmware_v_%s" % version, UPLOADCMD=save_firmware_for_ota
)
