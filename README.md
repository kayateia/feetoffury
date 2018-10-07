# Feet of Fury - Dreamcast indie game, source release!
### Original code Copyright 2003, Cryptic Allusion, LLC
### Subsequent modifications in 2005 for FoFRemix, independently by Kayateia

## Foreword

So yeah, this year marks the *15th* anniversary of the game's release. WTF, amirite?

For those who weren't following it at the time, Feet of Fury was an indie game
released by the Dreamcast by the same people who mostly shepherded the KallistiOS
project at the time (Cryptic Allusion, and more specifically, moi).

This game was a labor of love, a real sprint to the finish line of a goal and
a desire that I've had since I was about 6 years old and first sat a TI BASIC
book on my lap while trying to write a silly game called "Crash".

I spent a while looking through the code, tweaking a few things here and there. Some
things I can't release, like the decryption keys for the data files (sorry everyone),
and some things I changed because I have changed, and life has moved on, and e.g.
I no longer think it's a great idea to post one's legal name on the internet. (Also
I no longer have the same name, so it's inaccurate anyway, so :P)


## Wait, what?

*runs away*


## What is "FoFRemix"?

A while back, I got it into my head to port FoF to PCs - macOS, Windows, etc - and
turn it into a new game with bug fixes, new features, better swap song support, etc.
I actually got pretty far with it. This code here is from that port. It's mostly
intact, though a few DC-specific pieces have been commented out. But whatever.
At some point I may go back and release the toolkit required to build the full
project, which is called Tiki. You might think of it as a sort of "KOS for PC",
almost. But of course, using the right APIs for a desktop OS.


## What is the future of Feet of Fury?

Probably nothing. But I think that's a great reason to get it out there now and
let fans take a look under the covers. I think that, glancing over it, there's
actually some pretty fantastic and innovative code in here for 2003. It had a lot
of useful features, like reference counting, and some DC-specific things that were
super ahead of their time (like Parallax/Tsunami, and threaded GUI rendering).
I hope people can find something interesting to look at in here.

Looking at my notes, it looks like I had some fun stuff planned for Remix:

* Revamped graphics
* Story mode where you can wander around and "fight" NPCs in dance battles
* Internet versus
* In-game step editor
* A kind of wacky new "puzzle mode" that was somehow rhythm based? I can't grok this

Oh well, maybe someone will pick it up and go with it. :) If I ever do, it's going
to be VR based :P


## Copyright notice
As the primary author of this code, and as one of the former owners of Cryptic
Allusion, and as someone who was granted permission to use this code as seen fit
into the future, I am releasing its contents to the public for educational
purposes. As per the license, however, you're permitted to do pretty much whatever
you want with it as long as credits are retained.

This code is released under the MIT License, which is listed in full below:

Copyright 2003 Cryptic Allusion, LLC
Copyright 2005, 2018 Kayateia

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
