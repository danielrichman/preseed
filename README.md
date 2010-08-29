# Ubuntu Minimal CD Preseeding

The scripts in this repo automate the process of embedding preseed files into
a mini.iso initrd.gz, along with the ability to add things like your ssh 
public key so that login is automatic as well.

They are very temperamental and experimental. Play with the results in 
virtual machines first, and note the "WITHOUT ANY WARRANTY" bit.

## Legal

<pre>
Copyright (C) 2010  Daniel Richman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

For a full copy of the GNU General Public License, 
see <http://www.gnu.org/licenses/>.
</pre>

## Usage

The remaster script is the only one you should run directly. If required, it 
downloads a mini.iso from archive.ubuntu.com
(see https://help.ubuntu.com/community/Installation/MinimalCD)
loop-mounts it and unpacks it. It will require sudo access to do this.

Edit files/settings/*, then:

<pre>
Usage ./remaster <preseed>
Where <preseed> is one of the items in files/preseeds/
</pre>

I apologise for the lack of documentation.
