#System directory
MAKE_DIR 	VA:/ isystem

#Kernel loader directory
MAKE_DIR	VA:/isystem/ kernelldrs
MAKE_FILE	VA:/isystem/kernelldrs/ kernelldrtm.bin
WRITE_FILE	VA:/isystem/kernelldrs/kernelldrtm.bin kernelldr/bin/kernelldrtm.bin
MAKE_FILE	VA:/isystem/kernelldrs/ kernelldrgm_640_480.bin
WRITE_FILE	VA:/isystem/kernelldrs/kernelldrgm_640_480.bin kernelldr/bin/kernelldrgm_640_480.bin
MAKE_FILE	VA:/isystem/kernelldrs/ kernelldrgm_800_600.bin
WRITE_FILE	VA:/isystem/kernelldrs/kernelldrgm_800_600.bin kernelldr/bin/kernelldrgm_800_600.bin
MAKE_FILE	VA:/isystem/kernelldrs/ kernelldrgm_1024_768.bin
WRITE_FILE	VA:/isystem/kernelldrs/kernelldrgm_1024_768.bin kernelldr/bin/kernelldrgm_1024_768.bin
MAKE_FILE	VA:/isystem/kernelldrs/ kernelldrgm_1280_1024.bin
WRITE_FILE	VA:/isystem/kernelldrs/kernelldrgm_1280_1024.bin kernelldr/bin/kernelldrgm_1280_1024.bin

#System command directory
MAKE_DIR	VA:/isystem/ bins

#Edit
MAKE_FILE	VA:/isystem/bins/ edit
WRITE_FILE	VA:/isystem/bins/edit apps/edit/bin/edit.bin

#Paint
MAKE_FILE	VA:/isystem/bins/ paint
WRITE_FILE	VA:/isystem/bins/paint apps/paint/bin/paint.bin

#System dump directory
MAKE_DIR	VA:/isystem/ dumps

#System data directory
MAKE_DIR	VA:/isystem/ datas

#System resource directory
MAKE_DIR	VA:/isystem/ resources
MAKE_DIR	VA:/isystem/resources/ images

#Images
MAKE_FILE	VA:/isystem/resources/images/ pointer.img0
WRITE_FILE	VA:/isystem/resources/images/pointer.img0 isystem/resources/images/pointer.img0
MAKE_FILE	VA:/isystem/resources/images/ bg_640_480.img0
WRITE_FILE	VA:/isystem/resources/images/bg_640_480.img0 isystem/resources/images/bg_640_480.img0
MAKE_FILE	VA:/isystem/resources/images/ bg_800_600.img0
WRITE_FILE	VA:/isystem/resources/images/bg_800_600.img0 isystem/resources/images/bg_800_600.img0
MAKE_FILE	VA:/isystem/resources/images/ bg_1024_768.img0
WRITE_FILE	VA:/isystem/resources/images/bg_1024_768.img0 isystem/resources/images/bg_1024_768.img0
MAKE_FILE	VA:/isystem/resources/images/ bg_1280_1024.img0
WRITE_FILE	VA:/isystem/resources/images/bg_1280_1024.img0 isystem/resources/images/bg_1280_1024.img0
MAKE_FILE	VA:/isystem/resources/images/ close_button_ns.img0
WRITE_FILE	VA:/isystem/resources/images/close_button_ns.img0 isystem/resources/images/close_button_ns.img0
MAKE_FILE	VA:/isystem/resources/images/ close_button_s.img0
WRITE_FILE	VA:/isystem/resources/images/close_button_s.img0 isystem/resources/images/close_button_s.img0
MAKE_FILE	VA:/isystem/resources/images/ hidden_button_ns.img0
WRITE_FILE	VA:/isystem/resources/images/hidden_button_ns.img0 isystem/resources/images/hidden_button_ns.img0
MAKE_FILE	VA:/isystem/resources/images/ hidden_button_s.img0
WRITE_FILE	VA:/isystem/resources/images/hidden_button_s.img0 isystem/resources/images/hidden_button_s.img0

#System font directory
MAKE_DIR	VA:/isystem/ fonts
MAKE_FILE	VA:/isystem/fonts/ default.ef
WRITE_FILE	VA:/isystem/fonts/default.ef isystem/fonts/default.ef

#System flags directory
MAKE_DIR	VA:/isystem/ flags
MAKE_FILE	VA:/isystem/flags/ cfs.flg
WRITE_FILE	VA:/isystem/flags/cfs.flg isystem/flags/cfs.flg

#System init file
MAKE_FILE	VA:/isystem/ init.bat
WRITE_FILE	VA:/isystem/init.bat isystem/init.bat

#System help file
MAKE_FILE	VA:/isystem/ help.txt
WRITE_FILE	VA:/isystem/help.txt isystem/help.txt