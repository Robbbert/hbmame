;PAC-MAN Hardware programming Ghost Hunt - Hurray Banana June 2016
;little game to demonstrate some basic concepts and structures
;used when writing assembly language code showing features of assembly language
;and feature support from the assembler
;developed using SJASM assembler and Visual Studio 2015
	device zxspectrum48										;allows savebin to work for creating the binaries - at the end of the file
															;instead of using OUTPUT before each org - just slightly easier to manage
; init code courtesy of cmonkey
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; constants defined here
; in general when declaring constants and symbolic addresses use a colon at end
; when using constants and symbolic addresses drop the colon, this allows you to search for declarations
; by putting the colon at the end of the symbol name, if you omitt it you will find all the references (which might be loads)
; labels starting with . are local labels and can be re-used in other modules
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RST_38H:					equ $ff							;constant for RST 38H instruction for setting vsync interrupt
irq_enable:					equ	$5000						;irq enable location in memory map, set 1 to enable 0 to disable
watchdog:					equ	$50c0						;watchdog - if not reset every few frames cpu resets - forcing a re-boot known as the watchdog barking
flip_screen_addr:			equ $5003						;if value 0 then normal tilemap, 1 forces flipped tilemap (does not affect sprites)
hardware_io_area:			equ $5000						;hardware mapped memory i/o base

tile_ram_play_area:			equ $4040						;main tilemap area starts here (top right)-  addr increase vertically down from top - top right tile
tile_ram_play_area_left:	equ $43a0						;main tilemap area top left tile
tile_ram_top_area:			equ $43c0						;top two rows above tilemap area- addr increase horizontally left from right 
tile_ram_play_area_last:	equ $43bf						;last address of main player area (bottom left corner)
tile_ram_top_vis:			equ $43c2						;first two and last two tiles not visible in each of 2 rows
tile_ram_top_vis_line1:		equ $43dd						;top left of visible top row sub to move to next column to right
tile_ram_top_vis_line2:		equ $43fd						;second text row at top
tile_ram_start:				equ $4000						;base address of tile ram
pallete_ram_start:			equ $4400						;base address of pallete ram
pallete_ram_main:			equ $4440						;main video area pallete start (after first two rows)
tile_ram_bot_area:			equ $4000						;bottom two rows below tilemap area- addr increase horizontally left from right
tile_ram_bot_vis:			equ $4002						;first two and last two tiles not visible in each of 2 rows
tile_ram_bot_vis_line1:		equ $401d						;top left of visible bottom row sub to move to next column to right
tile_ram_bot_vis_line2:		equ $403d						;top left of last border row
user_ram_start:				equ $4c00						;start of user allocated RAM
ram_top:					equ $4ff0						;this is where the stack needs to start at

;-note 8 sprite registers exist but only the middle 6 are actually used by the hardware
; $4ff2-3,$4ff4-5,$4ff6-7,$4ff8-9,$4ffa-b,$4ffc-d - for the spritenum, flip and palette
; $5062-3,$5064-5,$5066-7,$5068-9,$506a-b,$506c-d - for the x and y positions

sprite_reg:					equ $4ff2						;sprite registers start here first byte is number bits 7-2, bit 1 is x flip, bit 0 is y flip
															;second byte lower nibble is pallete to use for sprite
spriteXY_reg:				equ $5062						;2 bytes starting here for each sprite holding x and y positions on screen


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; sprite block offsets organised for easy transfer to sprite registers pos and pal, then pos data, then velocities xy
; offsets to use when indexed addressing - set index register to sprite0, sprite1 etc...
; ld ix,sprite2
; ld a,(ix + spr_x)
; would allow you to access x position of sprite
; this data is cleverly interlaced so sprite number, palettes and xy positions can be easily copied over to the sprite registers each frame
; these equates allow us to access the interlaced elements without having to worry about how they are interlaced using indexing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
spr_num:				equ $0								;sprite number from rom 5f most significant 6 bits bit 1 x flip, bit 0 y flip
spr_pal:				equ $1								;palette for sprite - lower nibble only 
spr_x:					equ $0c								;x position of sprite
spr_y:					equ $0d								;y position of sprite
spr_vel_x:				equ $18								;x velocity of sprite
spr_vel_y:				equ $19								;y velocty of sprite
spr_tile_addr:			equ $24								;tile address sprite occupies (2 bytes)
spr_tile_x:				equ $30								;x tile position (0,0) is top right corner
spr_tile_y:				equ $31								;y tile position (27,31) is bottom left corner

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; colour pallete constants for pen 3 colour
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
black:					equ 0;		(hidden)
red:					equ 1;		(blinky)
pink:					equ 3;		(pinky)
cyan:					equ 5;		(inky)
orange:					equ 7;		(clyde)
yellow:					equ 9;		(pac-man)
grey:					equ $f;		(off-white)
blue:					equ $10;	(scared ghost)
brown:					equ $15;	(poo ghost)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; data to be held in ram defined here                                 ;
; allocation of bytes for ram area assembler calculates			      ;
; addresses based on org statement and user_ram_start ($4c00)		  ;
; these are in affect our variables									  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; db define byte (8 bit value) creates space and sets value
; dw define word (16 bit value) creates space and sets value
; ds declares space for a number of bytes (often specified using a calculation for the assembler)
; defm defines a byte
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	org user_ram_start										;allocate this data at user_ram_start $4c00
waitvsync:				db 0								;if 1 then interrupt is finished and main loop can continue 
frame_counter:			db 255								;value that increments every frame for stable animation and frame counts - doesn't reset just wraps so mods work consistently
frame_number:			db 0				 				;holds a frame number in lsB and second counter in msB 60 to 0 resets to 60 when gets to zero 
seconds_counter:		db 0								;quick ref to seconds part

full_tile:				db 0								;tile to use by bitmap renderer
save_reg:				db 0								;temp storage for an 8 bit register
save_sp:				dw 0								;storage area for stack when messing about with it
joystick_state:			db 0								;holds previous state in upper nibble and current state in lower nibble  IN1 $5000
start_state:			db 0								;holds current state of input byte IN1 - player 2 joystick and start buttons $5040
flip_screen:			db 0								;holds status of flip screen (as i think the location $5003 is write only - it always reads back as ff)
sprite_data:			ds 4*6 + 2*6 + 2*6 + 2*6			;space for the 6 sprites to store their sprite data 
															;4 bytes for sprite, pallete, x and y
															;2 bytes for x and y velocity
															;2 bytes for tile addr (not used)
															;2 bytes for tilex and tiley
															;60 bytes in total for this data

msg_top_addr			dw 0								;holds address of top changing display text 
msg_one_pos:			dw 0								;start addr of top text
msg_bot_addr:			dw 0								;holds index of in bottom_list text to display at bottom of screen
bot_counter:			db 0								;counter for bottom scrolling message display

mode:					db 0								;what mode is the system in 1 - title screen, 2 is game, 4 is end of game
high_score:				ds 4								;current high score (again 4 bcd bytes)
current_score:			ds 4								;space for current players score 4 bytes in bcd format (8 digits)
game_time:				db 0								;holds the game time

flipx:					equ 1								;x flip bit for sprites
flipy:					equ 0								;y flip bit for sprites
sprite0:				equ sprite_data						;quick reference constants to load index registers with for sprite access
sprite1:				equ sprite_data + 2
sprite2:				equ sprite_data + 4
sprite3:				equ sprite_data + 6
sprite4:				equ sprite_data + 8
sprite5:				equ sprite_data + $a
sprite_pos:				equ sprite_data + spr_x
sprite_vels:			equ sprite_data + spr_vel_x
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Code starts here
; when Z80 becomes active the PC will hold address 0
; address zero points to the first byte in ROM
; we place a jump instruction here so we can execute our code
; that will initialise the hardware and setup initial data structures
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
				
        		org  0										;tell assembler to assemble from address 0
				jp	start									;jump to initialisation code

				org $38										;assemble from $38 (where the vblank interrupt will jump to)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; vertical blank interrupt routine
; interrupt generated when screen has finished drawing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
vblank:			di											;disable any interrupts (so we don't get interrupted)
				push	af									;save accumulator, flags and registers we are going to destroy
				push	ix									 
				push	hl									
				push	de									
				push	bc									
				
				xor	a										;clear accumulator
				ld	(irq_enable),a							;disable ints on the board
				ld	(watchdog),a							;reset the watchdog
		
				;increment frame counter						
				ld a,(frame_counter)						;get current value
				inc a										;increment 
				ld (frame_counter),a						;and store

				;sort out frame counter
				ld hl,(frame_number)						;get 16 bit frame counter - l holds frame, h holds seconds
				dec l										;reduce number of frames by 1
				jp p,dont_reset_frame_counter				;only reset if it goes negative

				ld l,$3b									;load back up with 59
				inc h										;add one to seconds

dont_reset_frame_counter:
				ld (frame_number),hl						;store new counter

				; main vblank routines
				call set_sprite_data						;copy sprite data over to registers
				call input_manager							;update input states
				;call flip_input								;check for screen flip inputs
vbend:			
				xor a										;clear a
				ld	(watchdog),a							;and reset the watchdog
				inc a										;enable interrupts again
				ld (irq_enable),a							; enable ints on the board
				pop	bc										;restore registers in reverse order of push
				pop	de
				pop	hl
				pop	ix
				pop	af
				ei											;enable interrupts again
				reti										;return from interrupt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; rest frame number to start second counting from 0 frame
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
reset_framenumber:
				ld a,$39									;set up frame number for 0 so seconds are reset
				ld (frame_number),a							;reset frame number
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; input manager packages the input data from the two io ports
; so it can be used to track button/joystick state in a nice format
;
; keeps stack of current and previous state of inputs
; so we can test whether they are just pressed, held or released
; input manager constants 0 is down 1 is up
; mask status byte first using these
; for joystick_state
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
inp_chk_up:				equ $11								;mask for looking at up (bits 4 and 0)
inp_chk_left:			equ $22								;mask for looking at left (bits 5 and 1)
inp_chk_right:			equ $44								;mask for looking at right (bits 6 and 2)
inp_chk_down:			equ $88								;mask for looking at down (bits 7 and 3)
;for start_state
inp_chk_p1st:			equ $50								;mask for looking at p1 start (bits 6 and 4)
inp_chk_p2st:			equ $a0								;mask for looking at p2 start (bits 7 and 5)

;comparison constants for player 1 and 2 start buttons
inp_p1_presd:			equ $40								;check for p1 start pressed bit 6 previous 1 -up bit 4 current down 0 )
inp_p2_presd:			equ $80								;check for p2 start pressed

inp_p1_relsd:			equ $10								;check for p1 start released bit 6 previous 0 -down bit 4 current up 1 )
inp_p2_relsd:			equ $20								;check for p2 start released

;comparison constants to check wether just pressed or released
inp_up_presd:			equ $10								;check for up pressed
inp_left_presd:			equ $20								;check for left pressed
inp_right_presd:		equ $40								;check for right pressed
inp_down_presd:			equ $80								;check for down pressed

inp_up_relsd:			equ $01								;check for up released
inp_left_relsd:			equ $02								;check for left released
inp_right_relsd:		equ $04								;check for right released
inp_down_relsd:			equ $08								;check for down released

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; manages processing of previous and current states, for pressed and released behaviour checking
; as well as allowing standard up and down checking
; gets new state and packs this into a byte with the previous state in the upper nibble
; uses a byte for joystick direction and a separate byte for the start buttons
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
input_manager:
				;bit 0 - 3 up,left,right,down
				ld a,(joystick_state)						;get joystick state
				add a,a
				add a,a
				add a,a
				add a,a										;shift lower nibble to upper nibble to move to previous state location
				and $f0										;clear lower nibble
				ld b,a										;store temporarily
				ld a,($5000)								;get IN0
				and $0f										;clear upper nibble
				or b										;combine current state with previous state PPPP CCCC
				ld (joystick_state),a						;and store new previous and current state

				;bit 5 is 1p start, bit 6 is 2pstart
				;do start button states - PPCC xxxx upper hi bits of upper nibble contain previous state, low bits of upper nibble contain current state
				ld a,(start_state)							;get start button state
				add a,a
				add a,a										;shift current two bits to previous position
				and $c0										;clear lower 2 bits of upper nibble - and the lower nibble
				ld b,a										;store temporaily
				ld a,($5040)								;get IN1
				rra											;shift two bits to low bits of upper nibble
				and $30										;clear all bits other than 4 and 5
				or b										;combine previous with current PP CC XXXX
				ld (start_state),a							;store previous and new states
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; not being used just a little mess at flipping screen tiles and also flipping sprites to maintain
; there relative position and velocities 
; check player 2 start button test this with up
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
flip_input:			
				ld a,(start_state)							;get state
				and inp_chk_p2st							;examine p2 start
				cp inp_p2_presd								;check for pressed 
				ret nz										;not held held 0 means pressed
				ld a,(flip_screen)							;get flip screen status value
				cpl											;flip bit
				ld (flip_screen),a							;store again
				ld (flip_screen_addr),a						;write to flip screen ram - this is a write only location so we need to maintain state ourselves
;Notes 
; sprites have to be flipped manually
; this loop works through each sprite it needs to:
; flip the x and y flip bits
; negate (*-1) the x and y velocities
; adjust and compliment the x and y positions
flip_all_sprites:
				ld de,$0d11									;amount to subtract from positions 0d for x and $11 for y
				ld c,$03									;mask for toggling flip bits (bits 1 and 0)
				
				ld ix,sprite_data							;load up sprite number data
				ld b,6										;need to do 6 sprites
.flip_bits:		ld a,c										;load up flip mask for x and y flip bits
				xor (ix + spr_num)
				ld (ix + spr_num),a							;store flipped version
				
				;addjust position
				ld a,(ix + spr_x)							;get x position
				sub d										;subtract offset
				cpl											;flip bits
				ld (ix + spr_x),a							;and store
				
				ld a,(ix + spr_y)							;get y position
				sub e										;subtract offset
				cpl											;flip bits
				ld (ix + spr_y),a							;and store

				;adjust velocities
				ld a,(ix + spr_vel_x)						;get x velocity
				neg											;and negate
				ld (ix + spr_vel_x),a						;and store

				ld a,(ix + spr_vel_y)						;get y velocity
				neg											;and negate
				ld (ix + spr_vel_y),a						;and store


				inc ix
				inc ix										;skip two bytes to move to next sprite block
				djnz .flip_bits								;perform loop until b is zero
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; copies sprite data from ram to the sprite registers
; places data from sprite variables into the sprite registers
; this happens once a frame, but for efficiency you could just manage this manually
; for each sprite using custom code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
set_sprite_data:	
				;first block of data is sprite number(+flip data) and pallete number
				ld hl,sprite_data							;point to sprite data in ram
				ld de,sprite_reg							;point to sprite registers
				ld bc,12									;do for all 6 sprites 2 bytes of data
				ldir										;copy byte (hl) -> (de) inc de and hl, dec bc stop when bc is 0
				
				
				;second block of data is the position data
				ld de,spriteXY_reg							;point to xy position regsiters
				ld bc,12									;do for all 6 sprites 2 bytes of data
				ldir										;copy byte (hl) -> (de) inc de and hl, dec bc stop when bc is 0

				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; THIS IS THE ENTRY POINT FROM THE JUMP AT ADDRESS 0;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; sets up z80 interrupt mode and clears all RAM		;
; then initialises code to display screen			;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start:			di											;disable ints on the CPU
				
				ld	sp,ram_top								;set up a new stack at the top of ram
				im	1										;set interrupt mode 1
				ld	a,RST_38H								;$ff is RST 38H instruction opcode
				out	(0),a									;set up the interrupt vector through port 0

	 			xor	a										;clear accumulator quickly 4 t-states rather than lda a,0 which is 7
	 			ld	(watchdog),a							;reset the watchdog
	
 	   			ld	hl,user_ram_start						;addr to copy from
 	   			ld	de,user_ram_start + 1					;addr to copy to
 	   			ld	bc,$3ef									;gonna loop this many times (size of RAM 1007 bytes)
 	   			ld	(hl),a									;clear (hl), which then gets copied through to de
 	   			ldir										;copy from (hl) to (de) and increment hl and de, decrementing bc, keep doing until bc = 0
 	   
				ld  a,$0f									;tile number of empty tile
 	   			ld	hl,tile_ram_start						;clear video ram
 	   			ld	de,tile_ram_start + 1
 	   			ld	bc,$3ff									;1k of tile ram
 	   			ld	(hl),a									;clear (hl), which then gets copied through to de
 	   			ldir										;copy from hl to de, decrementing bc, keep doing until bc = 0
 	   	
				ld  a,$1									;pallete 1
 	   			ld	hl,pallete_ram_start					;set all colour ram to palette 1
 	   			ld	de,pallete_ram_start + 1
 	   			ld	bc,$3ff									;1k of palette ram
 	   			ld	(hl),a									;clear (hl), which then gets copied through to de
 	   			ldir										;copy from hl to de, decrementing bc, keep doing until bc = 0

 	   			xor a										;quickly clear accumulator
 	   			ld	hl,sprite_reg							;clear sprite ram
 	   			ld	b,16									;16 bytes need clearing
clear_spr_ram:	ld	(hl),a									;store (zero)
				inc	hl										;move to next address
 	   			djnz clear_spr_ram							;decrement b and branch if not zero yet
 	   	
				ld	hl,hardware_io_area						;clear hardware mapped memory from $5000 to $5007
				ld	b,8										;do 8 bytes worth
clear_loop1:	ld	(hl),a									;store a at location
				inc	hl										;increment to next location
				djnz clear_loop1							;loop until b is zero

set_highscore:
				ld hl,$100									;set the 4 low bcd digits
				ld (high_score),hl							;low bytes are here		
				ld h,0										;clear h
				ld (high_score + 2),hl						;set the upper 4 bcd digits to zero

				xor	a										;clear a
				ld	(watchdog),a							;reset the watchdog, to stop it barking
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end of hardware initialisation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

				; enable interrupts and start main loop
				ld a,1
				ld	(irq_enable),a							;enable ints on the board by writing 1 to this address
				ei											;enable ints on the CPU

				jp start_title_mode							;setup title screen

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; logic to be performed every frame - main logic of code
; needs to check to see what mode game is in first so it can run
; the correct game loop function
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;some constants for mode setting
title_mode:		equ $1
game_mode:		equ $2
game_over:		equ $4

mainloop:		
				ld a,(mode)									;get mode
				cp title_mode								;is it title mode
				jp z,title_logic							;yes
				cp game_mode								;is it game mode
				jp z,game_logic								;yes
				cp game_over								;is it game over
				jp z,game_over_logic						;yes
wait:
				halt										;finished processing let's wait for vsync
				jp mainloop									;perform logic again

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; sets up title screen
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start_title_mode:
				ld a,title_mode								;pick title mode
				ld (mode),a									;and store

				;setup scrolling message
				ld hl,top_mess								;store start position for top scrolling text
				ld (msg_one_pos),hl							;address of first character

				ld hl,tile_ram_top_vis_line1				;address of top left row of top border
				set 2,h										;force to pallete ram
				ld a,grey									;colour pallete for white (pen 3)
				ld b,$1c									;number of tiles to fill (28 entire columns)
				call fill_border_area_row					;set colour for entire top row

				;setup top changing message
				ld hl,top_list								;point to first item in top list address
				ld (msg_top_addr),hl
				ld hl,tile_ram_top_vis_line2				;address of top left row of top border
				set 2,h										;force to pallete ram
				ld a,yellow									;colour pallete for white (pen 3)
				ld b,$1c									;number of tiles to fill (28 entire columns)
				call fill_border_area_row					;set colour for entire top row

				;setup bottom changing message
				ld hl,bottom_list							;point to last item in the list for bottom text drawing routine so we cycle round straight away
				ld (msg_bot_addr),hl						;store in lookup
				ld hl,tile_ram_bot_vis_line1				;address of top left row of top border
				set 2,h										;force to pallete ram
				ld a,yellow									;colour pallete for white (pen 3)
				ld b,$1c									;number of tiles to fill (28 entire columns)
				call fill_border_area_row					;set colour for entire top row

				;clear any data on second to top line
				ld de,msg_blank								;store text addr for instruction message
				ld hl,tile_ram_top_vis_line2				;vram address for second line of tiles
				call borders_blit_text_only					;write text to border

				;clear any data on second to last line
				ld de,msg_blank								;store text addr for instruction message
				ld hl,tile_ram_bot_vis_line1				;vram address for first of line of tiles
				call borders_blit_text_only					;write text to border

				;set static instruction message on bottom line
				ld de,msg_instruction						;store text addr for instruction message
				ld hl,tile_ram_bot_vis_line2				;vram address for first of bottom line of tiles
				ld c,grey									;colour (white)
				call borders_blit_text						;write text to border

				ld hl,spr_initial_title_data				;address of title screen sprite data
				call set_sprites							;do initial transfer of sprite data from rom to ram

				ld a,blue									;blue pallete for screen
				call fill_screen_area_colour				;fill entire screen with palette 16

				ld hl,title_screen_column_data				;load hl with address of title screen data
				ld a,$0e									;render with full block
				call draw_pixel_map_to_screen				;call code to render title screen data

				jp wait										;finished setup goto wait section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; performs logic for title mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
title_logic:
				ld a,(start_state)							;get state
				and inp_chk_p1st							;examine p1 start
				cp inp_p1_presd								;check for pressed 
				jp z,start_game								;0 means pressed so start game

				call apply_sprites_velocities				;apply velocities to sprites

				;display work
				call scrolling_message						;top line of scrolling text	
				call process_top_message_text				;top line of changing text
				call bot_display_upd						;update text display at bottom of screen
				;call flash_pellets							;code to flash pellets	
				;call flash_get_ready						;flash get ready text

				jp wait										;finished setup goto wait section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; sets up game screen
; clears screen 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start_game:
				ld a,game_mode								;pick game mode
				ld (mode),a									;and store

				ld a,grey									;blue pallete for screen
				call fill_screen_area_colour				;fill entire screen with palette 16

				ld hl,title_screen_column_data				;load hl with address of title screen data
				ld a,$7f									;render with dippled block
				call draw_pixel_map_to_screen				;call code to render title screen data

				;clear old messages
				ld de,msg_blank								;store text addr for instruction message
				ld hl,tile_ram_top_vis_line2				;vram address for 2nd line of tiles
				call borders_blit_text_only					;write text to border
				ld de,msg_blank								;store text addr for instruction message
				ld hl,tile_ram_bot_vis_line1				;vram address for first of bottom line of tiles
				call borders_blit_text_only					;write text to border

				ld de,msg_scoring							;store text addr for scoring message
				ld hl,tile_ram_top_vis_line1				;vram address for first of bottom line of tiles
				ld c,grey									;colour (yellow)
				call borders_blit_text						;write text to border

				;show debug text
				ld de,msg_debug								;store text addr for instruction message
				ld hl,tile_ram_bot_vis_line1				;vram address for first of bottom line of tiles
				ld c,cyan									;colour (yellow)
				call borders_blit_text						;write text to border

				;set static instruction message on bottom line
				ld de,msg_quit_game							;store text addr for instruction message
				ld hl,tile_ram_bot_vis_line2				;vram address for first of bottom line of tiles
				ld c,yellow									;colour (yellow)
				call borders_blit_text						;write text to border

				ld hl,spr_initial_game_data					;addr of game sprite data
				call set_sprites							;do initial transfer of sprite data from rom to ram

				ld a,$60									;set game counter to 60 (in bcd)
				ld (game_time),a							;and store

				ld hl,0										;clear
				ld (current_score),hl						;reset player score
				ld (current_score + 2),hl					;reset player score

				call reset_framenumber						;set frame number to 0 so we start at a whole second

				jp wait										;finished setup goto wait section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; performs logic for game mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
game_logic:
				ld a,(start_state)							;get state
				and inp_chk_p2st							;examine p2 start
				cp inp_p2_presd								;check for pressed 
				jp z,start_title_mode						;0 means pressed so quit to title mode


				call player_one_movement					;control player 
				call shoot									;allow player to shoot if bullet not in use
				call apply_sprites_velocities				;apply velocities to sprites
				call calc_tile_locations					;works out tile positions of sprites
				call bullet_collision_detection				;check for collisions between bullet at other sprites
				call ghost_collision_detection				;check to see if ghost have hit player
				call time_tick								;sort out game time

				;display work
				call show_time								;displays timer
				call show_score								;displays player and high score
				call debug_output							;some debug values for player positions

				jp wait										;finished setup goto wait section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; checks to see if whole second has passed
; if so reduces time, if it's zero goes to game over
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
time_tick:
				ld a,(frame_number)							;get frame number
				and a										;see if it is zero
				ret nz										;if not then second is not up yet

				ld a,(game_time)							;get game time
				dec a										;reduce by one
				daa											;correct for bcd
				ld (game_time),a							;store new time
				ret nz										;if not zero then stop now
															;fall through to game over setup
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; performs logic for game over mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;constants
game_over_top_blank:	equ $42eb							;address of top blank
game_over_location:		equ $42ec							;address to draw game over
game_over_bot_blank:	equ $42ed							;address of bottom blank
start_game_over:
				ld a,game_over								;set mode to game over
				ld (mode),a									;and store

				ld a,10										;load up game over time for 10 seconds
				ld (game_time),a							;and store, so score is displayed for 10 seconds

				ld hl,time_location							;load up time display addr
				ld a,$0f									;blank tile
				ld (hl),a									;store tile over 1st digit
				dec hl										;move to 2nd digit
				ld (hl),a									;and overwrite

				ld a,yellow									;set yellow pallette
				call fill_screen_area_colour				;and fill screen
				ld a,$8f									;full tile is checkerboard
				call fill_screen_area_working_slow			;draw tile to screen

				;hide all sprites
				ld ix,sprite0								;get first sprite
				ld b,6										;gonna do for all 6 sprites
.hide_loop:
				call hide_sprite							;hide this sprite
				inc ix										;move to next sprite
				inc ix										;2 bytes along
				djnz .hide_loop								;if b not zero loop
				pop bc										;remove return address from stack as we are aborting the call
				
				ld de,msg_game_over							;get text addr for game message
				ld hl,game_over_location					;vram address for text
				ld c,red									;colour red
				call main_blit_text_and_colour				;write text to main game area

				ld de,msg_gameover_blank					;get text addr for blank portion
				ld hl,game_over_top_blank					;vram address for blank
				call main_blit_text_only					;write blank to main game area

				ld de,msg_gameover_blank					;get text addr for blank portion
				ld hl,game_over_bot_blank					;vram address for blank
				call main_blit_text_only					;write blank to main game area

				jp wait										;wait for next game loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; performs logic for game over mode
; just a countdown timer so no other functions needed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
game_over_logic:
				ld a,(frame_number)							;get frame number
				and a										;see if it is zero
				jp nz,wait									;if not then second is not up yet

				ld a,(game_time)							;get game time
				dec a										;reduce by one
				ld (game_time),a							;store new time
				jp z,start_title_mode						;if zero then go back to title				

				jp wait										;finished setup goto wait section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; works out which tile each sprite is in so tile based
; collision can be performed if required
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
calc_tile_locations:
				ld ix,sprite0								;load index with first sprite
				ld b,6										;work out for all 6 sprites
.truncate_next:
				ld a,(ix + spr_x)							;get current x position of this sprite
				rrca										;divide by 8 (integer - size of a tile in pixels) 
				rrca										;achieved through 3 rotate rights of a then mask top 3 bits
				rrca 
				and $1f										;(this is instead of doing 3 right shifts 'srl a' as it's quicker to rotate then mask)
				ld (ix + spr_tile_x),a						;store x tile

				ld a,(ix + spr_y)							;get current y position of sprite
				rrca										;divide by 8 (integer - size of a tile in pixels) 
				rrca										;achieved through 3 rotate rights of a then mask top 3 bits
				rrca 
				and $1f										;(this is instead of doing 3 right shifts 'srl a' as it's quicker to rotate then mask)
				ld (ix + spr_tile_y),a						;store y tile
				
				inc ix										;move pointer to next sprite block (2 bytes)
				inc ix
				djnz .truncate_next							;decrement b and loop if b is not zero
				
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; displays time
; in sensible locations on 2nd row
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;constants
time_location:		equ $43f1								;address in vram to place time
show_time:
					ld hl,time_location						;vram address to write to
					ld a,(game_time)						;get game time
					ld c,a									;byte needs to be in c
					call show_hex							;use hex display as this is a bcd number (so it just works)
					ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; displays player score and high score
; in sensible locations on 2nd row
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;constants
hi_score_location:		equ $43e9
show_score:

				ld hl,tile_ram_top_vis_line2				;output on start of top line

				ld a,(current_score + 3)					;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score
				ld a,(current_score + 2)					;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score
				ld a,(current_score + 1)					;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score
				ld a,(current_score)						;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score

				ld hl,hi_score_location						;output on right hand side
				ld a,(high_score + 3)						;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score
				ld a,(high_score + 2)						;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score
				ld a,(high_score + 1)						;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score
				ld a,(high_score)							;get player score (hi byte first - little endian)
				ld c,a
				call show_hex								;output hi byte of score

				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; shows debug output
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;constants
debug_start_loc	equ $4017	
debug_output:
				;player position
				ld ix,sprite0								;get base address of player sprite data
				ld a,'X'									;load up ascii for X					
				ld hl,debug_start_loc						;output text from here
				ld (hl),a									;output
				dec hl										;move to next location
				ld c,(ix + spr_x)							;get x value of player sprite
				call show_hex								;output X value

				ld a,$0f									;space tile
				ld (hl),a									;output
				dec hl										;move along to the right one tile 
				ld a,'Y'									;load up ascii for Y					
				ld (hl),a									;output
				dec hl										;move along right one tile
				ld c,(ix + spr_y)							;get y value of player sprite
				call show_hex

				ld a,$0f									;space tile
				ld (hl),a									;output
				dec hl										;move along to the right one tile 
				ld a,'T'									;load up ascii for T					
				ld (hl),a									;output
				dec hl										;move along to the right one tile 
				ld a,'X'									;load up ascii for Y					
				ld (hl),a									;output
				dec hl										;move along right one tile
				ld c,(ix + spr_tile_x)						;get x tile value of player sprite
				call show_hex

				ld a,$0f									;space tile
				ld (hl),a									;output
				dec hl										;move along to the right one tile 
				ld a,'T'									;load up ascii for T					
				ld (hl),a									;output
				dec hl										;move along to the right one tile 
				ld a,'Y'									;load up ascii for Y					
				ld (hl),a									;output
				dec hl										;move along right one tile
				ld c,(ix + spr_tile_y)						;get y tile value of player sprite
				jp show_hex									;jump instead of call allows us to return from this function
															;when the show_hex function ends saving 10 cycles
				;end of test code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; looks for overlapping tiles between bullets and any ghosts
; only does this if the bullet is visible (not destroyed)
; this is not a very granular way of collision detecting
; could do region AABB overlapping for finer collision detection
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bullet_collision_detection:
				ld ix,sprite5								;point at bullet sprite
				call is_sprite_hidden						;determine if it's hidden
				ret z										;don't do collision detection if it is
				
				;check to see if bullet at left edge or bottom of screen and hide if it is
				ld a,(ix + spr_tile_x)						;get bullet tile x position
				cp $2										;is tile x equal to or bigger than 2
				jp p,.check_y								;no bigger than or equal to 2 so potentially on screen
.hide_bullet:
				jp hide_sprite								;call hide sprite and let it return for us (so we don't need to do two ret instructions)
.check_y:
				ld a,(ix + spr_y)							;get bullet y position
				cp $7										;has it reached 7 											
				jr z,.hide_bullet							;yes so hide bullet and stop processing collisions - performs a pc relative jump
				cp $6
				jr z,.hide_bullet							;yes so hide bullet and stop processing collisions - performs a pc relative jump

				;good to continue collision detection with the ghosts
				push ix										;put ix into iy
				pop iy										;by using stack

				ld h,(iy + spr_tile_x)						;load tile x of bullet
				ld l,(iy + spr_tile_y)						;load tile y of bullet

				ld ix,sprite1								;check the first of four ghosts
				ld b,4										;4 sprites to check
.test_loop:				
				call is_sprite_hidden						;see if visible
				jp z,.next_sprite							;ignore if not

				ld d,(ix + spr_tile_x)						;get x tile of sprite
				ld a,h										;get x tile of bullet
				cp d										;is sprite tile x the same
				jp nz,.next_sprite							;if not same skip to next sprite

				ld a,l										;get y tile of bullet
				ld e,(ix + spr_tile_y)						;get y tile of sprite
				cp e										;is the tile y the same (collision if it is)
				jp nz,.next_sprite							;no so go to next sprite

				ld a,15										;place ghost at right edge of screen
				ld (ix + spr_x),a							;location of sprite position

				ld ix,sprite5								;setup bullet sprite
				call hide_sprite							;hide bullet
				jp score_points								;increase points obtained (this will return for us also)
.next_sprite:
				inc ix										;move to next sprite
				inc ix										;2 bytes further along
				djnz .test_loop								;keep going until b is zero
.stop_checking:
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; looks for overlapping tiles between ghosts and player
; if they are colliding then ghost is killed and player loses points
; can't go lower than 0 ?
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ghost_collision_detection:
				ld ix,sprite5								;point at bullet sprite
				call is_sprite_hidden						;determine if it's hidden
				ret z										;don't do collision detection if it is
				
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; increases points scored by player
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
score_points:
				ld hl,current_score							;get start of score (lo byte first)
				ld a,(hl)									;load into accumulator
				add $90										;add 90 pts - written in bcd
				daa											;correct for bcd overflow
				ld (hl),a									;store new value
				ld b,3										;need to see if the carry propogates through remaining three bcd bytes
				ld c,0										;gonna add 0 so quicker to load into register
.addloop:
				inc hl										;move to next highest byte
				ld a,(hl)									;load into accumulator
				adc c										;add zero with carry taken into account
				daa											;adjust flags to correct for bcd overflow
				ld (hl),a									;store new bcd pair
				djnz .addloop								;continue loop until b is zero
				
				;now check high score to see if bigger start with high bytes
				ld b,4
				ld de,current_score + 4						;get high byte first (point one too far as we dec straight away)
				ld hl,high_score + 4						;get high byte first (point one too far as we dec straight away)
.check_loop:
				dec de										;move to next most significant bytes
				dec hl										;for score and high score
				ld a,(de)									;load score byte
				cp (hl)										;compare with highscore byte (does subtract but doesn't affect a register)
				daa											;correct for bcd numbers (in case we have 80 and above as these would appear as -ve numbers)
				ret m										;not bigger so stop checking
				jp nz,.new_high_score						;if numbers not the same then we have a new high score so stop checking
				djnz .check_loop:							;do for all 4 bytes if necessary (until b = 0)

				;if here score is better so copy score to highscore
.new_high_score:
				ld hl,current_score							;set current score location in hl
				ld de,high_score							;set highscore location in de and copy
				ld bc,4										;copy 4 bytes from (hl) to (de)
				ldir
				
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; let's player shoot in direction tank is facing
; if bullet is already travelling then it is restarted (bit like Gorf)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
shoot:
				ld ix,sprite5								;specify bullet sprite
				;call is_sprite_hidden						;check to see if sprite is visible
				;ret nz										;don't even attempt to shoot if bullet still visible

				ld a,(start_state)							;get state
				and inp_chk_p1st							;examine p1 start
				cp inp_p1_presd								;check for pressed 
				ret nz										;not held held 0 means pressed

				ld iy,sprite0								;get player sprite data
				ld a,(iy + spr_x)							;get x position
				ld (ix + spr_x),a							;and copy to bullet

				ld a,(iy + spr_y)							;get y position
				ld (ix + spr_y),a							;and copy to bullet

				ld a,$27 * 4								;set sprite for bullet multiply by 4 to place in top 6 bits
				ld (ix + spr_num),a							;store in sprite data

				ld a,yellow									;load up pacman colour
				ld (ix + spr_pal),a							;and store in bullet pallete data

				ld a,(iy + spr_vel_x)						;get x direction for player (from velocity that is not used)
				add a										;quick mulitply by 2
				ld (ix + spr_vel_x),a						;and store for bullet
				add a
				add a										;multiply again (by 4) and update bullet position to put on edge of sprite (rather than centre)
				add a,(ix + spr_x)							;add offset to current x value
				ld (ix + spr_x),a							;and store new position

				ld a,(iy + spr_vel_y)						;get y direction for player (from velocity that is not used)
				add a										;quick mulitply by 2
				ld (ix + spr_vel_y),a						;and store for bullet
				add a
				add a										;multiply again (by 4) and update bullet position to put on edge of sprite (rather than centre)
				add a,(ix + spr_y)							;add offset to current y value
				ld (ix + spr_y),a							;and store new position
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; hides a sprite (by setting it's colour palette to 0)
; ix contains base address of sprite data 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
hide_sprite:
				xor a										;set a to zero
				ld (ix + spr_pal), a						;store 0 palette for sprite
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; checks to see if a sprite is visible (it's colour is not 0)
; ix contains base address of sprite data 
; sets status register, zero indicates sprite is hidden
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
is_sprite_hidden:											
				ld a,(ix + spr_pal)							;pick up palette
				and a										;test for zero
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; checks to see if a sprite is visible (it's colour is not 0)
; ix contains base address of sprite data 
; sets status register, non zero indicates sprite is visible
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
is_sprite_visible:
				ld a,(ix + spr_pal)							;pick up pallete in use
				cpl											;compliment
				and a										;set flags
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; shows a sprite (by setting it's colour palette to a)
; ix contains base address of sprite data 
; a contains colour pallete to use
;
; to be honest this is written for clarity, you'd do this inline in reality
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
show_sprite:
				ld (ix + spr_pal), a						;store chosen palette for given sprite
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; picks up joystick moves
; sets appropriate sprite for horizontal and vertical movement
; and sets flip bits to show sprite correctly
; also alters sprite position by one pixel
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
player_one_movement:
				ld ix,sprite0								;point to sprite0 data block

				ld a,(joystick_state)						;get joystick state
				ld b,a										;and save in b
				and inp_chk_left							;examine left
				and a										;check for pressed 
				jp nz,.right								;not held held 0 means pressed
				inc (ix + spr_x)							;increase x value
				
				xor a										;clear vertical movement
				ld (ix + spr_vel_y),a						
				inc a										;+1 for negative x velocity
				ld (ix + spr_vel_x),a						;and store

				ld a,$23*4									;horizontal tank *4 to get sprite number in top 6 bits
				ld (ix),a									;set horizontal tank sprite
				res flipx,(ix)								;no flip x - set to 0 - clear
				jp .upndown

.right:			ld a,b										;get state again
				and inp_chk_right							;examine right
				and a										;check if pressed
				jp nz,.upndown								;it's non zero so isn't pressed
				dec (ix + spr_x)

				xor a										;clear vertical movement
				ld (ix + spr_vel_y),a						

				dec a										;-1 for positive x velocity
				ld (ix + spr_vel_x),a						;and store

				ld a,$23*4									;horizontal tank *4 to get sprite number in top 6 bits
				ld (ix),a									;set horizontal tank sprite
				set flipx,(ix)								;flip x - set to 1

.upndown:		ld a,b										;get state again
				and inp_chk_up								;examine up
				and a										;check if pressed
				jp nz,.down									;it's non zero so isn't pressed
				inc (ix + spr_y)							;move up

				xor a										;clear horizontal movement
				ld (ix + spr_vel_x),a						
				inc a										;+1 for negative y velocity (was 0 add 1)
				ld (ix + spr_vel_y),a						;and store

				ld a,$22*4									;vertical tank *4 to get sprite number in top 6 bits
				ld (ix),a									;set vertical tank sprite
				res flipy,(ix)								;no flip y - set to 0 - clear
				ret											;don't bother checker down

.down:			ld a,b										;get state again
				and inp_chk_down							;examine down
				and a										;check if pressed
				ret nz										;it's non zero so isn't pressed so finish
				dec (ix + spr_y)							;move up

				xor a										;clear horizontal movement
				ld (ix + spr_vel_x),a						
				dec a										;-1 for positive y velocity (was 0 dec to ff)
				ld (ix + spr_vel_y),a						;and store

				ld a,$22*4									;veritcal tank *4 to get sprite number in top 6 bits
				ld (ix),a									;set vertical tank sprite
				set flipy,(ix)								;no flip y - set to 0 - clear


				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; displays a portion of text from a message at the top of the screen
; updates every 8 frames
; address of start position is loaded into hl
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
scrolling_message:
				ld a,(frame_counter)						;get frame counter see if it's modable
				and $7										;mask all but low bits - scroll every 8 frames
				ret nz										;don't update text unless mod 8 is 0

				ld hl,(msg_one_pos)							;get text addr of first character to show
				inc hl										;move to next character
				ld a,(hl)									;load first char to see if it's zero
				and a										;is it zero?
				jp nz,.render_scrolling_message				;if not then display text
				ld hl,top_mess								;if it is then reset to start position for top text

.render_scrolling_message:
				ld (msg_one_pos),hl							;store text addr for next time
				ex de,hl									;swap hl into de so character address is in de
				ld	hl,tile_ram_top_vis_line1				;vram address to display to
				;ld	c,$3f									;colour (white)
				call borders_blit_text_only					;render text without setting colour pallete
				ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;routine to update text displays that change
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
process_top_message_text: 
				ld hl,(frame_number)						;get frame number see if it's 60
				ld a,l										;check frame counter
				and a										;to see if it is zero
				ret nz										;don't update if not frame 0
				ld a,h										;get second counter
				and a										;make sure zero flag possible
				ret z										;get out if seconds 0
				ld a,(top_mess_seconds)						;get seconds timer bits to look at for message
				and h										;look at these bits in h, if they are zero change message
				ret nz										;don't update if not zero result

				ld hl,(msg_top_addr)						;get addr of message to display
				ld e,(hl)									;remember lo-byte first in memory
				inc hl										;move to hi-byte
				ld d,(hl)									;place in d
				inc hl										;point to next addr in array of addresses
				ld a,d										;load hi-byte into for or'ing with the lo-byte
				or e										;see if address is zero (if we or we can test full zero easy)
				jp nz,process_text_top_message_text			;if it isn't display the message

				ld hl,top_list								;reset to first item and re-load
				ld e,(hl)									;remember lo-byte first in memory
				inc hl										;move to hi-byte
				ld d,(hl)									;place in d
				inc hl										;point to next addr in array of addresses

process_text_top_message_text:
				ld (msg_top_addr),hl						;store next message to display
				ld	hl,tile_ram_top_vis_line2				; draw on second line at top
				ld	c,$9									; palette 9 (third colour yellow)
				call borders_blit_text_only					;write text to border area
				ret

bot_display_upd:	
				ld hl,(frame_number)						;get frame number see if it's 60
				ld a,l										;check frame number
				and a										;to see if it's zero
				ret nz										;don't update if not frame 0
				
				ld a,(bot_mess_seconds)						;get seconds interval for message change
				and h										;and with number of seconds gone by
				ret nz										;only update if on a 2 second interval, jump onto top display text
				
				ld hl,(msg_bot_addr)						;get addr of message to display
				ld e,(hl)									;remember lo-byte first in memory
				inc hl										;move to hi-byte
				ld d,(hl)									;place in d
				inc hl										;point to next addr in array of addresses
				ld a,d										;load hi-byte into for or'ing with the lo-byte
				or e										;see if address is zero (if we or we can test full zero easy)
				jp nz,process_text_bot_text					;if it isn't display the message

				ld hl,bottom_list							;reset to first item and re-load into de
				ld e,(hl)									;remember lo-byte first in memory
				inc hl										;move to hi-byte
				ld d,(hl)									;place in d
				inc hl										;point to next addr in array of addresses

process_text_bot_text:
				ld (msg_bot_addr),hl						;store next message to display
				ld	hl,tile_ram_bot_vis_line1				;tile address to start writing at
				ld	c,$15									;palette 21 (third colour tan)
				call borders_blit_text_only					;write text to border area
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; HELPER FUNCTIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; takes a byte and outputs the hex value to top or bottom rows only
; hl holds address of vram to place data
; c holds value to output
; a little tricky as tiles stored in ascii, so need to do adjustments to move between
; the stock digits and the 6 hex additional digits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
show_hex:
		ld b,2												;do this twice
.nibbleout:
		xor a												;clear carry
		ld a,c												;get byte to output
		rlca
		rlca
		rlca
		rlca												;rotate 4 times to swap nibbles
		ld c,a												;store temp for next time
		and $f												;mask 
		cp $a												;are we bigger than 9
		jp p,.alphahex										;adjust for a to f
		add 48												;it's 0-9 so add 48 for tile number
		jp .output											;now render to screen
.alphahex:
		add 55												;add 55 (so a becomes 65 - ASCII A)
.output:
		ld (hl),a											;write tile value to vram
		dec hl												;move to next column on the right for lower nibble
		djnz .nibbleout
		ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; generic text blitting routine for normal areas of screen (28x32 main playfield)
; text strings MUST be zero terminated
; on entry
; de points to text string
; hl points to address in vram to print at
; c = palette colour of tile
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main_blit_text_and_colour:	
				ld	a,(de)									;get the byte at de
				inc	de										;increment the text pointer
				or	a										;test for string terminator 
				ret z										;return if we've finished this string
				
        		ld	(hl),a									;write the byte to vram
        		set	2,h										;point hl at colour ram by setting bit 2 of h
        		ld	a,c										;get colour byte into a
        		ld	(hl),a									;write the byte to colour ram
        		ld	a,l										;low byte of vram/colour address into A
        		sub	32										;subtract 32 to move across the screen by 1 char position
        		ld	l,a										;store back to l
        		res	2,h										;point hl back at vram by clearing bit 2 of h
        		ld	a,h										;handle the carry from the SUB instruction (if there was one)
        		sbc	a,0										;subtract zero and the carry flag from a
        		ld	h,a										;store back to d
        		jp	main_blit_text_and_colour				;loop until we read a zero terminating byte
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; generic text blitting routine for normal areas of screen (28x32 main playfield)
; text strings MUST be zero terminated
; on entry
; de points to text string
; hl points to address in vram to print at
; can do this as we don't have mess about with pallette ram
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main_blit_text_only:	
				push bc										;save bc as we are gonna nuke it
				ld bc,32									;column skip amount
.char_loop:
				ld	a,(de)									;get the byte at de
				inc	de										;increment the text pointer
				or	a										;test for string terminator 
				jp z,.finish								;return if we've finished this string
				
        		ld	(hl),a									;write the byte to vram
				sbc hl,bc									;move to next column
        		jp	.char_loop								;loop until we read a zero terminating byte
.finish:
				pop bc										;retrieve bc
				ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; writes text to the top or bottom two rows
; on entry
; de points to text string
; hl points to address in vram to print at
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
borders_blit_text:	
				ld b,$1d									;maximum number of chars to draw in a line (28)
.blit_loop:
				ld	a,(de)									;get the byte at HL
				inc	de										;increment the text pointer
				or	a										;test for string terminator (0)
				ret z										;return if we've finished this string
				
				dec b										;check maximum characters has not been met
				ret z										;return if we have
		
        		ld	(hl),a									;write the byte to vram
        		set	2,h										;point DE at colour ram by setting bit 2 of D
        		ld	a,c										;get colour byte into A
        		ld	(hl),a									;write the byte to colour ram
        		res	2,h										;point DE back at vram by clearing bit 2 of D
				dec hl										;move to next column
        		jp	.blit_loop								;loop until we read a zero terminating byte

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; writes text to the top or bottom two rows
; hl contains vram address to write to
; de holds address of zero terminated string
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
borders_blit_text_only:	
				ld b,$1d									;maximum number of chars to draw in a line (28)
.blit_loop:
				ld	a,(de)									;get the byte at HL
				inc	de										;increment the text pointer
				or	a										;test for string terminator (0)
				ret z										;return if we've finished this string
				
				dec b										;check maximum characters has not been met
				ret z										;return if we have
		
        		ld	(hl),a									;write the byte to vram
				dec hl										;move to next column
        		jp	.blit_loop								;loop until we read a zero terminating byte		


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; fills an entire row in either the top or
; bottom border with the value in a
; a contains tile/pallete number to write
; b contains number of tiles to fill
; hl contains tile ram or palette
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
fill_border_area_row:
.write 			ld	(hl),a									;store value at current address
				dec hl										;move right one column
 	   			djnz .write									;loop back if we have more to do
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; xor's an entire row in either the top or
; bottom border with the a value
; b contains number of tiles to fill
; c contains value to xor
; hl contains tile ram or palette
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
xor_border_area_row:
.write 			
				ld a,c										;load xor mask
				xor (hl)									;xor with current tile/pallete addr contents
				ld	(hl),a									;write new value
				dec hl										;move right one column
 	   			djnz .write									;loop back if we have more to do
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; fills the entire screen pallete ram with given pallete number
; a contains pen colour to fill tiles with
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
fill_screen_area_colour:
				ld	hl,pallete_ram_main						;colour ram
 	   			ld	de,pallete_ram_main + 1					;one on from hl so we effectively drag the value at hl through vram
 	   			ld	bc,$37f									;all but one address needed as we manually set the first one below
 	   			ld	(hl),a									;write first value to hl
 	   			ldir										;copy (hl) -> (de), inc hl, inc de, dec bc, loop until bc = 0
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; a contains tile number
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
fill_screen_area_working_slow:
				ld	hl,tile_ram_play_area					;tile ram
 	   			ld	de,tile_ram_play_area + 1				;one on from hl so we effectively drag the value at hl through vram
 	   			ld	bc,$37f									;all but one address needed as we manually set the first one below
 	   			ld	(hl),a									;write first value to hl
 	   			ldir										;copy (hl) -> (de), inc hl, inc de, dec bc, loop until bc = 0
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; fills a row with given tile/pallete
; hl holds start address to fill
; a holds the tile/palette colour to use
; b holds number of tiles/palettes to draw
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
fill_screen_area_row:
				ld de,-32									;skip between columns 32 bytes down to move right
.write_loop:	
				ld (hl),a									;store tile/pallete value 
				add hl,de									;move right (back 32 bytes)
				djnz .write_loop							;have we done the correct number of writes?
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; xor's a row with given value - useful for flashing tiles
; hl holds start address to fill
; b holds number of tiles/palettes to draw
; c holds value to xor with tile/pallete
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
xor_screen_area_row:
				ld de,-32									;skip between columns 32 bytes down to move right
.write_loop:	
				ld a,c										;load c into
				xor (hl)									;and xor value at hl with a
				ld (hl),a									;store tile/pallete value 
				add hl,de									;move right (back 16 bytes)
				djnz .write_loop							;have we done the correct number of writes?
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; takes a wasteful raw tilemap and copies to tile ram
; hl contains addr of raw tilemap data
; map in this format needs a byte per tile and therefore takes up 896 bytes of rom
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
draw_raw_map_to_screen:
 	   			ld	de,tile_ram_play_area					;main draw area of screen
 	   			ld	bc,$380									;copy 896 bytes raw
 	   			ldir										;(hl) -> (de) inc hl, inc de, dec bc, stop if bc = 0	
				ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; sp contains address +1 to push 2 tiles into
; it's quicker to use stack pointer to push 2 bytes than using (hl) and increment
; de gets destroyed
; b contains full block to use
; a contains bitmap for tiles
; 2 leftmost bits are dropped into carry for testing
; to be used as a block of 4 macro calls to process an entire byte (8 rows)
; need 4 of these blocks to complete an entire column
; 
; Macro's allow us to create unrolled loops, this processes two bits of data
; they have to be populated with local labels as the code is substituted by the assembler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
tile_empty_block:		equ $0f
tile_full_block:		equ $0e

	MACRO DECODE_AND_PUSH_TWO_TILES																	
				add a										;get msb into carry						
				jp c,.fullfirst								;is it a block (1)						
				ld d,tile_empty_block						;no so set empty tile					
				jp .next									;look at next bit						
.fullfirst:		ld d,b										;set first to a full tile				

.next:			add a										;get msb into carry for msb				
				jp c,.fullnext								;is it a block (1)						
				ld e,tile_empty_block						;no sp set set 2nd tile to empty tile	
				jp .pushnow									;go and perform the push				
.fullnext:		ld e,b										;set second tile to full tile			
.pushnow:		push de										;store the tiles on screen				
	ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; this draws a column at a time in reverse (bottom to top) to take advantage of stack pushes
; takes a bitmap encoded pixel map and renders to screen in solid tiles
; on entry hl holds address of pixel map
; a holds tile to use in place of block
; $0e is a full block
; $7f is hash and $8f is checker pattern
; uses the macro above 4 times - to process a full byte of data
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
draw_pixel_map_to_screen:
				ld (save_sp),sp								;save stack pointer as we are going to use it to store to tile ram
				ld (full_tile),a							;save full tile
				ld sp,tile_ram_play_area_last + 1			;set stack pointer to end of main video area + 1 - as push will pre decrement the address
				ld b,28 * 4									;28 columns of data to process 4 bytes per column
.process_byte:	
				ld a,b										;copy a to b to move to ram
				ld (save_reg),a								;temporarily store b
				ld a,(full_tile)							;get full tile
				ld b,a										;and put in b
				
				ld a,(hl)									;load bitmap for next eight blocks from map
				inc hl										;move to next bitmap ready for next loop
															
															;use 4 copies of the macro to process entire byte
				DECODE_AND_PUSH_TWO_TILES					;process bits 7 and 6
				DECODE_AND_PUSH_TWO_TILES					;process bits 5 and 4
				DECODE_AND_PUSH_TWO_TILES					;process bits 3 and 2
				DECODE_AND_PUSH_TWO_TILES					;process bits 1 and 0
				
				ld a,(save_reg)								;get saved value
				ld b,a										;restore b
				djnz .process_byte							;loop to process next byte of map if b is not zero
				ld sp,(save_sp)								;restore stack pointer
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; show pacman maze for start of intro
; this is not used but shows how to use a raw map (in this case pac_map_raw:)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;some constants
playerone_txt_pallete:	equ $468c
getready_txt_pallete:	equ $4652
show_pac_maze:
				ld a,$10									;pallete for pacman maze
				call fill_screen_area_colour				;fill entire screen with palette 16

				ld hl,pac_map_raw							;render pac tilemap to screen
				call draw_raw_map_to_screen					;data includes tiles for get ready player one text

				;set colour for two text parts of pacman maze, player one and get ready!
				ld hl,playerone_txt_pallete					;address of player one pallete
				ld b,10										;ten palette addresses please
				ld a,5										;cyan palette (inky)
				call fill_screen_area_row					;call function to do it

				ld hl,getready_txt_pallete					;address of ready? pallete
				ld b,6										;six palette addresses please
				ld a,9										;pacman palette (yellow)
				jp fill_screen_area_row						;jump to function to do it and let it do return for this function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; flashes every 32 frames the tiles showing ready
; example of flashing tiles using counters and xor helper function
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;some constants

flash_get_ready:
				ld a,(frame_counter)						;get frame count for flashing animation changes 
				and $1f										;and with 31 to see if it is zero
				ret nz										;change every 32 frames (mod 32) only

				ld hl,getready_txt_pallete					;address of ready? pallete
				ld b,6										;six palette addresses please
				ld c,9										;pacman palette (yellow)
				jp xor_screen_area_row						;jump to toggle code and let that do the return (quicker)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; flashing routine for power pellets
; pellets at colour pallete addresses $4784,$4464,$4798,$4479
; just toggle palette between 0 and 16 by xor $10
;
; not used but flashes the 4 power pellets drawn in the pac_map_raw data
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;few constants
pellet_topright:		equ $4464
pellet_botright:		equ $4478
pellet_topleft:			equ $4784
pellet_botleft:			equ $4798

flash_pellets:
				ld a,(frame_counter)						;get frame rate for flashing animation changes (for power pills)
				and $f										;check lower nibble to see if it is zero
				ret nz										;change every 16 frames (mod 16)
				
				;flash the power pellets
				ld b,$10									;load b with 16 (colour palette of pellets)
				
				ld hl,pellet_topright						;get address of palette
				ld a,b										;put 16 in a
				xor (hl)									;xor with current pallete
				ld (hl),a									;write xor value back
				
				ld hl,pellet_botright						;get address of palette
				ld a,b										;put 16 in a
				xor (hl)									;xor with current pallete
				ld (hl),a									;write xor value back
				
				ld hl,pellet_topleft						;get address of palette
				ld a,b										;put 16 in a
				xor (hl)									;xor with current pallete
				ld (hl),a									;write xor value back
				
				ld hl,pellet_botleft						;get address of palette
				ld a,b										;put 16 in a
				xor (hl)									;xor with current pallete
				ld (hl),a									;write xor value back
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; performs intial copy of sprite data over
; to the sprite ram area for game mode
; hl holds address to copy sprite data from
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
set_sprites:
				ld de,sprite_data							;address of sprite data in ram
				ld bc,4 * 6									;get ready to copy all bytes (6 on hardware)
				ldir										;copy data to ram

				ld de,sprite_vels							;get velocity ram location
				ld bc,2 * 6								;copy velocities pairs for each sprite
				ldir
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; updates positions based on velocities
; for each of the sprites other than player sprite
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
apply_sprites_velocities:
				ld de,sprite_vels + 2						;get velocity of first sprite - skip sprite 0
				ld hl,sprite_pos + 2						;get position data for first sprite - skip sprite 0
				ld b,5										;gonna update 5 sprites positions (not including player position)
spr_vel_loop:
				ld a,(de)									;get x velo of sprite
				add a,(hl)									;add x position to x velocity
				ld (hl),a									;save new x position

				inc de										;move to y velocity
				inc hl										;move to y position
				
				ld a,(de)									;get y velo of sprite
				add a,(hl)									;add y position to y velocity
				ld (hl),a									;save new y position

				inc de										;move to next sprite velocities
				inc hl										;move to next sprite positions

				djnz spr_vel_loop							;keep going if sprites remaining to be updated
				ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Data starts here
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
				;;align to start off second 4k rom .6f
        		org  $1000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; new sinewave data with a +- 127 range [ 127 * Sin(x) ] represents one full period 0 to 2Pi (0-359 degrees)
; 360/256 is fractional accessor or 2Pi/256
; positions 0 to 63 represent the range  0 to 89 degrees
; positions 64 to 127 represent the range  90 to 179 degrees
; positions 128 to 191 represent the range 180 to 269 degrees
; positions 192-255 represent the range  270 to 359 degrees
;
; to access use the following format:
;
;	ld hl,sinewave		;point to base of sinewave table
;	ld l,63				;pick up 
;	ld a,(hl)			;load 128 * sin(90) approximately
;	;use shifts to get smaller numbers
;	sra	a				;divide number by 2
;
; this is page aligned so we can use a single register to access the sine value
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
sinewave: 
	defm 000,003,006,009,012,016,019,022
	defm 025,028,031,034,037,040,043,046
	defm 049,051,054,057,060,063,065,068
	defm 071,073,076,078,081,083,085,088
	defm 090,092,094,096,098,100,102,104
	defm 106,107,109,111,112,113,115,116
	defm 117,118,120,121,122,122,123,124
	defm 125,125,126,126,126,127,127,127
	defm 127,127,127,127,126,126,126,125
	defm 125,124,123,122,122,121,120,118
	defm 117,116,115,113,112,111,109,107
	defm 106,104,102,100,098,096,094,092
	defm 090,088,085,083,081,078,076,073
	defm 071,068,065,063,060,057,054,051
	defm 049,046,043,040,037,034,031,028
	defm 025,022,019,016,012,009,006,003
	defm 000,-003,-006,-009,-012,-016,-019,-022
	defm -025,-028,-031,-034,-037,-040,-043,-046
	defm -049,-051,-054,-057,-060,-063,-065,-068
	defm -071,-073,-076,-078,-081,-083,-085,-088
	defm -090,-092,-094,-096,-098,-100,-102,-104
	defm -106,-107,-109,-111,-112,-113,-115,-116
	defm -117,-118,-120,-121,-122,-122,-123,-124
	defm -125,-125,-126,-126,-126,-127,-127,-127
	defm -127,-127,-127,-127,-126,-126,-126,-125
	defm -125,-124,-123,-122,-122,-121,-120,-118
	defm -117,-116,-115,-113,-112,-111,-109,-107
	defm -106,-104,-102,-100,-098,-096,-094,-092
	defm -090,-088,-085,-083,-081,-078,-076,-073
	defm -071,-068,-065,-063,-060,-057,-054,-051
	defm -049,-046,-043,-040,-037,-034,-031,-028
	defm -025,-022,-019,-016,-012,-009,-006,-003

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; mutliplication look up page aligned - 256 bytes 15x15 times table
; page aligned as previous table was 256 bytes (an entire page)
; this is also an entire page (256 bytes as it includes 0 * n)
; to use num1 * num2 do the following:
;
;	ld hl,multiplication_lookup	;set hl pointing at lookup table
;	ld a,(num1)					;load num 1 into a
;	add a,a						;multiply a by 16 this points to start of each numbers times table
;	add a,a				
;	add a,a
;	add a,a
;	add a,(num2)				;add on the number to multiply by to get the offset along table
;	ld l,a						;put this into l
;	ld a,(hl)					;load value from lookup table into a
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
multiplication_lookup:
	defm 000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000
	defm 000,001,002,003,004,005,006,007,008,009,010,011,012,013,014,015
	defm 000,002,004,006,008,010,012,014,016,018,020,022,024,026,028,030
	defm 000,003,006,009,012,015,018,021,024,027,030,033,036,039,042,045
	defm 000,004,008,012,016,020,024,028,032,036,040,044,048,052,056,060
	defm 000,005,010,015,020,025,030,035,040,045,050,055,060,065,070,075
	defm 000,006,012,018,024,030,036,042,048,054,060,066,072,078,084,090
	defm 000,007,014,021,028,035,042,049,056,063,070,077,084,091,098,105
	defm 000,008,016,024,032,040,048,056,064,072,080,088,096,104,112,120
	defm 000,009,018,027,036,045,054,063,072,081,090,099,108,117,126,135
	defm 000,010,020,030,040,050,060,070,080,090,100,110,120,130,140,150
	defm 000,011,022,033,044,055,066,077,088,099,110,121,132,143,154,165
	defm 000,012,024,036,048,060,072,084,096,108,120,132,144,156,168,180
	defm 000,013,026,039,052,065,078,091,104,117,130,143,156,169,182,195
	defm 000,014,028,042,056,070,084,098,112,126,140,154,168,182,196,210
	defm 000,015,030,045,060,075,090,105,120,135,150,165,180,195,210,225

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; some messages to display
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
							;0123456789012345678901234567 - for alignment purposes
msg_instruction:	defm	"WELCOME TO  THIS LITTLE GAME",0
msg_quit_game:		defm    " 2P START  TO QUIT TO TITLE ",0
msg_debug:			defm	"DEBUG                       ",0
msg_scoring:		defm	"1 UP       TIME     HI SCORE",0
msg_game_over:		defm	"  GAME    OVER  ",0
msg_gameover_blank:	defm	"                ",0
msg_blank:			defm	"                            ",0 ;use this label to also refer to a blank message

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; data for the top line scrolling message
; terminated with a zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
top_mess:			
					defm	"                             GHOST HUNT               "
					defm	"        SHOOT AS MANY GHOSTS AS POSSIBLE IN 60 SECONDS                   "
					defm	"        CONTROLS                   "
					defm	"        PRESS 1P START TO SHOOT                   "
					defm	"        JOYSTICK CONTROLS TANK                   "
					defm	0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; a list of message width of the screen (28 columns)
; to display
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bot_mess_seconds:	defm 0	; change every second
bot_mess_1:			defm	"  PRESS 1P START TO BEGIN   ",0
bot_mess_2:			defm	" BY HURRAY BANANA JUNE 2016 ",0
					;		 0123456789012345678901234567 - for alignment purposes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; this is the addressess of the messages to display (above), 
; the code looks for an address of zero to restart the list
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bottom_list:		defw	bot_mess_1,msg_blank,bot_mess_2,msg_blank,0

;this is currently disabled but is basically a flicking message for the second row - where the timer and score sit
top_mess_seconds:   defm	1								;alter message every 2 seconds
top_mess_1:			defm	"         SIMPLE GAME        ",0
top_mess_2:			defm	"        DEMONSTRATION       ",0
top_mess_3:			defm	"  IN Z80 ASSEMBLY LANGUAGE  ",0
top_mess_4:			defm	"DESIGNED TO SHOW FEATURES OF",0
top_mess_5:			defm	"  ASSEMBLY LANUAGE SUCH AS  ",0
top_mess_6:			defm	"       ADDRESSING MODES     ",0
top_mess_7:			defm	"      USE OF INTERRUPTS     ",0
top_mess_8:			defm	"          SHIFTING          ",0
top_mess_9:			defm	"       LOOKUP TABLES        ",0
top_mess_a:			defm	"      MASKING   AND SO ON   ",0
top_mess_b:			defm	" ASSEMBLER FEATURES SUCH AS ",0
top_mess_c:			defm	"       USE OF CONSTANTS     ",0
top_mess_d:			defm	"     SYMBOLIC ADDRESSESS    ",0
top_mess_e:			defm	"     MACROS AND COMMENTS    ",0
top_mess_f:			defm	"THIS CODE IS DESIGNED TO RUN",0
top_mess_10:		defm	" ON ACTUAL  PACMAN HARDWARE ",0
top_mess_11:		defm	"FULLY COMMENTED  SOURCE CODE",0
top_mess_12:		defm	"     AVAILABLE ON REQUEST   ",0
top_mess_13:		defm	"  CREATED BY HURRAY BANANA  ",0
top_mess_14:		defm	"          JUNE 2016         ",0
					;		 0123456789012345678901234567 - for alignment purposes
					;addressess of message display, in order required - notice blank messages have been inserted
top_list:			defw	top_mess_1,top_mess_2,top_mess_3,msg_blank,top_mess_4,top_mess_5,top_mess_6,top_mess_7,top_mess_8,top_mess_9,top_mess_a
					defw	msg_blank,top_mess_b,top_mess_c,top_mess_d,top_mess_e,msg_blank,top_mess_f,top_mess_10,top_mess_11,top_mess_12,top_mess_13,top_mess_14
					defw	msg_blank,0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;column ordered data for generating the title screen
;bitmap for title screen - each bit is a solid tile when rendered
;top row is left hand column working from bottom to top
;full screen 28 rows of 4 bytes (32 bits for each of the 32 rows of the main screen area)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
title_screen_column_data:
	defm $00,$00,$00,$3E
	defm $00,$00,$00,$41
	defm $00,$00,$00,$49
	defm $00,$00,$00,$49
	defm $00,$00,$00,$7A
	defm $00,$00,$00,$00
	defm $00,$00,$00,$7F
	defm $00,$3F,$80,$04
	defm $FE,$77,$F0,$04
	defm $10,$7B,$F8,$78
	defm $10,$3B,$FC,$00
	defm $10,$17,$3C,$38
	defm $FE,$37,$3E,$44
	defm $00,$7B,$FE,$44
	defm $78,$7B,$FE,$44
	defm $80,$37,$3E,$38
	defm $40,$17,$3C,$00
	defm $F8,$3B,$FC,$08
	defm $00,$7B,$F8,$54
	defm $F8,$77,$F0,$54
	defm $08,$3F,$80,$54
	defm $08,$00,$00,$20
	defm $F0,$00,$00,$04
	defm $00,$00,$00,$3E
	defm $08,$00,$00,$44
	defm $7C,$00,$00,$24
	defm $88,$00,$00,$00
	defm $48,$00,$00,$00

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; initial settings for sprites in title mode
; sprite data sprite number multiplied by 4 (upper 6 bits refer to sprite number)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
spr_initial_title_data:
	;sprite number * 4 to but into top 6 bits (0-63) flips then in bit 0 and 1, and pallete byte
	defm $23*4, 1 ; sprite 0, flipped in x, palette 1
	defm $3f*4, 1 ; sprite 1
	defm $3f*4, 3 ; sprite 2
	defm $3f*4, 5 ; sprite 3
	defm $3f*4, 7 ; sprite 4
	defm $3f*4, 0 ; sprite 5		;bullet sprite invisible
	;starting positions x,y
	defm 140, 72	;sprite 0
	defm 180, 200	;sprite 1
	defm 160, 200	;sprite 2
	defm 140, 200	;sprite 3
	defm 120, 200	;sprite 4
	defm 100, 200	;sprite 5
	;sprite velocities x,y
	defm 0,0		;sprite 0
	defm 1,0		;sprite 0
	defm 1,0		;sprite 0
	defm 1,0		;sprite 0
	defm 1,0		;sprite 0
	defm 0,0		;sprite 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; initial settings for sprites in game mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
spr_initial_game_data:
	;sprite number * 4 to but into top 6 bits (0-63) flips then in bit 0 and 1, and pallete byte
	defm $23*4, 1 ; sprite 0, flipped in x, palette 1
	defm $3f*4, 1 ; sprite 1
	defm $3f*4, 3 ; sprite 2
	defm $3f*4, 5 ; sprite 3
	defm $3f*4, 7 ; sprite 4
	defm $3f*4, 0 ; sprite 5		;bullet sprite invisible
	;starting positions x,y
	defm 140, 72	;sprite 0
	defm 15, 200	;sprite 1
	defm 15, 150	;sprite 2
	defm 15, 100	;sprite 3
	defm 15, 50	;sprite 4
	defm 0, 0	;sprite 5
	;sprite velocities x,y
	defm 1,0		;sprite 0		;set velocity pointing left for bullet to pick up if stationary at start and shoot
	defm 2,1		;sprite 1
	defm 1,-1		;sprite 2
	defm -1,1		;sprite 3
	defm -2,-1		;sprite 4
	defm 0,0		;sprite 5

;holds a rough approximation of the pac-man tilemap in column major format, this is not compressed
;each line of bytes represents a column of the pac-man maze tiles (as re-engineered by me in rom 5e)
;starts with right hand column and works left (basically turn your head to your left shoulder)
;first column all those 032 values are the top row and so on
pac_map_raw: 
	defm 032,228,230,230,230,230,230,230,230,230,232,032,032,032,239,032,240,032,032,032,228,230,230,230,230,235,237,230,230,230,230,232
	defm 032,239,169,169,171,169,169,169,169,169,240,032,032,032,239,032,240,032,032,032,239,169,169,169,171,234,241,169,169,169,169,240
	defm 032,239,169,246,255,249,169,246,249,169,240,032,032,032,239,032,240,032,032,032,239,169,246,249,169,247,248,169,246,249,169,240
	defm 032,239,169,234,032,241,169,234,241,169,240,032,032,032,239,032,240,032,032,032,239,169,234,241,169,169,169,169,234,241,169,240
	defm 032,239,169,234,032,241,169,234,241,169,240,032,032,032,239,032,240,032,032,032,239,169,234,250,255,255,249,169,234,241,169,240
	defm 032,239,169,247,254,248,169,247,248,169,247,230,230,230,248,032,247,230,230,230,248,169,247,254,254,254,248,169,234,241,169,240
	defm 032,239,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,234,241,169,240
	defm 032,239,169,246,255,249,169,246,255,255,255,255,255,255,249,032,246,255,255,255,249,169,246,249,169,246,255,255,252,241,169,240
	defm 032,239,169,234,032,241,169,247,254,254,253,251,254,254,248,032,247,254,254,254,248,169,234,241,169,247,254,254,253,241,169,240
	defm 032,239,169,234,032,241,169,169,169,169,234,241,069,032,032,032,032,032,032,032,032,169,234,241,169,169,169,169,234,241,169,240
	defm 032,239,169,234,032,241,169,246,249,169,234,241,078,058,231,231,231,060,032,246,249,169,234,241,169,246,249,169,234,241,169,240
	defm 032,239,169,247,254,248,169,234,241,169,247,248,079,240,032,032,032,239,063,234,241,169,247,248,169,234,241,169,247,248,169,240
	defm 032,239,169,169,169,169,169,234,241,169,032,032,032,240,032,032,032,239,089,234,241,169,169,169,169,234,241,169,169,169,169,240
	defm 032,044,255,255,255,249,169,234,250,255,255,249,082,155,032,032,032,239,068,234,250,255,255,249,032,234,250,255,255,249,169,240
	defm 032,045,254,254,254,248,169,234,251,254,254,248,069,155,032,032,032,239,065,234,251,254,254,248,032,234,251,254,254,248,169,240
	defm 032,239,169,169,169,169,169,234,241,169,032,032,089,240,032,032,032,239,069,234,241,169,169,169,169,234,241,169,169,169,169,240
	defm 032,239,169,246,255,249,169,234,241,169,246,249,065,240,032,032,032,239,082,234,241,169,246,249,169,234,241,169,246,249,169,240
	defm 032,239,169,234,032,241,169,247,248,169,234,241,076,059,230,230,230,061,032,247,248,169,234,241,169,247,248,169,234,241,169,240
	defm 032,239,169,234,032,241,169,169,169,169,234,241,080,032,032,032,032,032,032,032,032,169,234,241,169,169,169,169,234,241,169,240
	defm 032,239,169,234,032,241,169,246,255,255,252,250,255,255,249,032,246,255,255,255,249,169,234,241,169,246,255,255,252,241,169,240
	defm 032,239,169,247,254,248,169,247,254,254,254,254,254,254,248,032,247,254,254,254,248,169,247,248,169,247,254,254,253,241,169,240
	defm 032,239,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,169,234,241,169,240
	defm 032,239,169,246,255,249,169,246,249,169,246,231,231,231,249,032,246,231,231,231,249,169,246,255,255,255,249,169,234,241,169,240
	defm 032,239,169,234,032,241,169,234,241,169,240,032,032,032,239,032,240,000,000,000,239,169,234,251,254,254,248,169,234,241,169,240
	defm 032,239,169,234,032,241,169,234,241,169,240,032,032,032,239,032,240,032,032,032,239,169,234,241,169,169,169,169,234,241,169,240
	defm 032,239,169,247,254,248,169,247,248,169,240,032,032,032,239,032,240,032,032,032,239,169,247,248,169,246,249,169,247,248,169,240
	defm 032,239,169,169,171,169,169,169,169,169,240,032,032,032,239,032,240,032,032,032,239,169,169,169,171,234,241,169,169,169,169,240
	defm 032,229,231,231,231,231,231,231,231,231,233,032,032,032,239,032,240,032,032,032,229,231,231,231,231,236,238,231,231,231,231,233


;nice and simple way of specifiying how to split the binary up. Has to be hard coded to file locations
;does an automatic ovewrite of files - needs to be at end of assembler file I think.
;save locations for data for each rom
	savebin "D:\PacmanCoding\roms\pacman\pacman.6e",$0000, $1000
	savebin "D:\PacmanCoding\roms\pacman\pacman.6f",$1000, $1000
	savebin "D:\PacmanCoding\roms\pacman\pacman.6h",$2000, $1000
	savebin "D:\PacmanCoding\roms\pacman\pacman.6j",$3000, $1000

;;try and auto launch mame debugger after assembly - does not like spaces in path names
	IF ((_ERRORS + _WARNINGS) = 0)
        SHELLEXEC "D:\PacmanCoding\mame.exe pacman -debug -rompath D:\PacmanCoding\roms\"
    ENDIF
	