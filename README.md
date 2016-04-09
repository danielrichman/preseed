# initrd embedding, Debian/Ubuntu CD preseeding

This repository used to contain bash scripts that helped create installer CDs with containing a `preseed.cfg`, or various files. You can still find those on the [bash-scripts](https://github.com/danielrichman/preseed/tree/bash-scripts) branch. They were too restrictive, and a rewrite in Python wasn't worth it. Ultimately, creating these CDs is rare, and doesn't need to be automated. So instead, this repo now contains instructions and a few examples.

## 1) Acquire the required components

You will need

```bash
sudo apt-get install debian-installer-8-netboot-amd64 fakeroot xorriso isolinux syslinux-common
```

(the last two only necessary if you want to produce CDs).

## 2) Vanilla installer

```bash
mkdir installer
cp /usr/lib/debian-installer/images/8/amd64/text/debian-installer/amd64/{linux,initrd.gz} installer/
```

## 3) Customise

The folder `extra` will contain things that will be added to the `initrd`.

```bash
mkdir extra
```

For example, a preseed file:

```bash
cp /path/to/my/preseed.cfg extra/preseed.cfg
```

Or, if you wanted to use the network-console (continue installation over SSH; see `netinstall-preseed.cfg`),

```bash
# add our public key to the installer, so we can log in
mkdir extra/.ssh
cp ~/.ssh/id_rsa.pub extra/.ssh/authorized_keys
# generate the installer's host keys in advance, so that we know what the fingerprints are.
# note that the installer will generate new ones for the final system.
mkdir -p extra/etc/ssh 
for type in rsa dsa ecdsa; do
    ssh-keygen -q -t $type -f extra/etc/ssh/ssh_host_${type}_key -N ''
    ssh-keygen -l -f extra/etc/ssh/ssh_host_${type}_key
done
```

and/or, if your network card requires binary blobs:

```bash
apt-get download firmware-bnx2
dpkg-deb --extract firmware-bnx2_*.deb extra/
```

## 4) Add `extra` to the initrd

```bash
gunzip installer/initrd.gz
(cd extra; find . | fakeroot cpio --quiet -F ../installer/initrd --append -o -H newc)
gzip installer/initrd
```

## 5) Make a CD

At this point, if you can direct boot, then `linux` and `initrd.gz` will suffice. Otherwise:

```bash
cp /usr/lib/ISOLINUX/{isohdpfx.bin,isolinux.bin} installer/
cp /usr/lib/syslinux/modules/bios/ldlinux.c32 installer/
```

Copy `isolinux.cfg` from this repository to `installer/isolinux.cfg`. Note the comment that enables the serial console, if you intend to install via OOB management / a VM / etc.

```bash
xorriso -outdev installer.iso                                \
    -map installer/ /                                        \
    -boot_image isolinux dir=/                               \
    -boot_image isolinux system_area=installer/isohdpfx.bin  \
    -boot_image isolinux partition_table=on
```
