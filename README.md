# Bare Metal C++ Application for BeagleBone Black

This is an example of the bare metal C++ application to be run on the BeagleBone Black board.

# Building

## Prerequisites

* Ubuntu OS
* Qt Build Suite:

    ```bash
    sudo apt install qbs
    ```

* ARM GCC:

    ```bash
    sudo apt install gcc-arm-none-eabi
    ```

## Compiling

* Setup QBS:

    ```bash
    make prepare
    ```

* Run `make`:

    ```bash
    make
    ```

* Find the output files (`*.bin` and `*.txt`) in the `release/install-root` directory.

# Running

## Prepare bootable SD card

We assume the SD-card is mounted to `/dev/sdb`. Modify first two lines if otherwise (e.g. `SD_DEV=/dev/mmcblk0` and `SD_PART=${SD_DEV}p1`).

```bash
SD_DEV=/dev/sdb
SD_PART=${SD_DEV}1

sudo dd if=/dev/zero of=$SD_DEV bs=1024 count=1024

sudo sfdisk --force $SD_DEV << EOF
,,b,*
EOF

umount $SD_PART
sudo mkfs.vfat -F 32 -n "boot" $SD_PART
```

## Get U-Boot

We use [Das U-Boot](https://en.wikipedia.org/wiki/Das_U-Boot) as a primary bootloader.

It consists of two parts: the first-stage bootloader (MLO) and the second-stage service (u-boot.img). Download their dedicated BeagleBone versions here:

http://rcn-ee.com/repos/bootloader/am335x_boneblack/

```bash
wget https://rcn-ee.com/repos/bootloader/am335x_boneblack/MLO-am335x_boneblack-v2019.04-r16 -O MLO
wget https://rcn-ee.com/repos/bootloader/am335x_boneblack/u-boot-am335x_boneblack-v2019.04-r16.img -O u-boot.img
```

Then copy `MLO` and `u-boot.img` to the SD-card.

See the mirror of these files in the `bin` directory inside this repo.

## Run the Example

Copy files build at the previous step (`*.bin` and `*.txt`) to the SD-card, insert it into the BeagleBone Black, and power up.

You will see the lit USER LED 0 (D2) and blinking USER LED 1 (D3).

# License

Source code is licensed under the [MIT License](LICENSE).
