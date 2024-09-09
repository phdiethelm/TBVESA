
                       TRaceBack'S VESA VBE 1.2+ Driver

                           for OrCAD Release IV/386+

                                 User's Manual



  1. What is "TRaceBack's VESA VBE 1.2+ Driver for OrCAD Release IV/386+" ?
  ==========================================================================


  TRaceBack's VESA VBE 1.2+ Driver for OrCAD Release IV/386+ is a display
  driver for the OrCAD Release IV and 386+ family of DOS products. When
  used in combination with a VESA VBE 1.2 or later standards compliant
  graphics hardware, it offers high resolution video modes to these products.

  Since the VESA VBE standard is widely adopted, the driver should work with
  almost any graphics hardware turning it into a generic display driver.

  Due to its early stage of development and the lack of proper documentation
  of OrCAD's display driver interface, the driver is intended to be used
  only in case none of the originally supplied display drivers work with the
  present graphics hardware.

  In order to futher improve on the stability and compatibility of the
  driver, users are encouraged to report any bug to traceback2004@web.de.



  2. Compatibility with OrCAD products
  ==========================================================================


  In its current state, the driver is compatible with all SDT IV, SDT 386+
  and PCB 386+ versions. It has also been used successfully with ESP 4.42
  and is expected to work with all ESP versions that originally shipped with
  386+ products. However, it does not work with older version of ESP, such
  as ESP 4.10 which came with SDT IV. Note that PCB II is currently not
  supported as well.

  Future releases of the driver are intended to be compatible with all
  Release IV and 386+ products.



  3. System Requirements
  ==========================================================================

  The driver requires at least a 386 compatible processor, a VESA VBE 1.2
  or later standards compliant graphics hardware and a resolution dependant
  amount of extended memory. The formula to determine extended memory
  requirements is :

        (horizontal resolution) * (vertical resolution) * 2 bytes

  DOS users should refer to their operating system's manual how to supply
  extended memory to an application.

  Windows NT/2000/XP users will most likely experience problems, because
  the NTVDM (NT Virtual DOS Machine) is providing only 1MB of extended
  memory to DOS applications by default, which is insufficient to run the
  driver in resolutions above 800x600. In case higher resolutions are
  desired, users must edit their _default.pif from the Windows system32
  directory to provide more extended memory. Alternatively the existing
  _default.pif can be replaced by a copy of the _default.pif from this
  archive. A backup should be made prior to overwriting the original
  _default.pif

  Since Windows 95/98/ME is providing sufficient amounts of extended memory
  by default, it shouldn't be necessary for users of these operating systems
  to make any changes to their system configuration.


  Although the VBE standard is wide-spread and specifications for the 3.0
  standard can be obtained free of costs from the Video Electronics Standard
  Association, a few incompatible and faulty implementations on the side of
  the graphics hardware exist. The situation is worse with cheap on-board
  graphics hardware. Stand-alone graphic cards from ATI, nVidia and Matrox
  are less often showing problems. Users serious about running OrCAD Release
  IV or 386+ products should ensure compatibility with graphics hardware
  before buying.

  If it's already too late for that users can still try to upgrade their
  VESA VBE implementation (which is most often part of the video bios) in
  case the vendor is supplying bug fixed updates. For older graphics
  hardware, the excellent and now free UNIVBE from Scitech might help. See
  the list of supported graphics chipsets before downloading.


  Users of GeForce4 (and probably others as well) based graphics hardware
  might experience problems with running the driver under Windows NT/2000/XP.
  The screen typically goes blank on these graphics cards or the driver
  refuses to load with a error messages like "Required video mode not found!".
  This can be fixed in most cases by installing the videoprt.sys driver from
  the archive's VIDEOPRT directory. Please not that this program was written
  by a different person and thus bug reports for it will be ignored.



  4. Installation
  ==========================================================================

  
  Copy all *.DRV files from the archive's BIN directory to your EPS's driver
  directory. On default installations, this will be C:\ORCADESP\DRV.

  In order to use the driver with a OrCAD Release IV/386+ product, you will
  have to type its name into the text field titled "Configured Display
  Driver". You can choose from one of the following drivers :


                        TB640.DRV (640x480)
                        TB800.DRV (800x600)
                        TB1024.DRV (1024x768)
                        TB1280.DRV (1280x1024)
                        TB1600.DRV (1600x1200)


  5. Compiling
  ==========================================================================

  The driver was compiled with OpenWatcom 1.3 which can be obtained free of
  costs from www.openwatcom.org. OpenWatcom 1.2 will work as well, as should
  any newer version of OpenWatcom. However, this cannot be guaranteed. If
  you want to be on the safe side, stick to OpenWatcom 1.3 for now.
