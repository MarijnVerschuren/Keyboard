About the reference design:
The CC2511 USB Nano stick reference design shows how to make an ultra compact USB stick with the CC2511 System-on-Chip, using a PCB antenna and single sided components mounting. 

The reference design contains schematics and layout files for the CC2511 USB Nano stick. The reference design demonstrates good techniques for decoupling and RF layout in miniaturized PCB designs with the CC2511. This is a 4-layer reference design with a compact balun and antenna integrated in the PCB. For optimum RF performance, the reference design must be copied exactly. In the reference design connections to the debug interface are excluded to save board space. Thus, the design is not suitable for development and the CC2511 must be pre-programmed before it is mounted onto the PCB.




XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X   	Artwork and documentation done by: 				X
X								X
X	TEXAS INSTRUMENTS NORWAY LPW              			X
X  								X
X	Address: Gaustadalléen 21    0349 OSLO                   		X
X	Phone  : (+47) 22 95 85 44   Fax :  (+47) 22 95 89 05    		X
X web: www.ti.com/lpw                               	 		 	X
X                                                          				X
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

PROJECT : 025106
PCB NAME : CC2511_USB_Bean_Dongle
REVISION: 1.1
DATE: 2008-05-06
QUANTITY:   See order 

PCB DESCRIPTION:4 LAYER PCB 0.6 MM
	  Dimensions in mil (0.001 inch)
                SINGLE SIDE SIDE SOLDER MASK,
                DOUBLE SIDE SILKSCREEN,
                8 MIL MIN TRACE WIDTH AND 5 MIL MIN ISOLATION.
	  Dielectric constant for 3.8 and 3.4
             
                 
FILE NAME            		DESCRIPTION                               			FILE TYPE
-------------------------------------------------------------------------------------------
***PCB MANUFACTURING FILES:
L1.SPL      LAYER 1 COMPONENT SIDE/POSITIV  			EXT. GERBER
L2.SPL      LAYER 2 INTERNAL (BURIED) LAYER		  	EXT. GERBER
L3.SPL			LAYER 3 INTERNAL (BURIED) LAYER    		EXT. GERBER
L4.SPL			LAYER 4 SOLDER SIDE/POSITIV        		EXT. GERBER
STOPCOMP.SPL		SOLDER MASK COMPONENT SIDE/NEGATIVE           	EXT. GERBER
STOPSOLD.SPL    SOLDER MASK SOLDER SIDE/NEGATIVE              	EXT. GERBER

SILKCOMP.SPL    SILKSCREEN COMPONENT SIDE/POSITIVE            	EXT. GERBER
SILKSOLD.SPL		SILKSCREEN SOLDER SIDE/POSITIVE			            EXT. GERBER

PASTCOMP.SPL  	SOLDER PAST COMPONENT SIDE/POSITIVE           	EXT. GERBER
PASTSOLD.SPL		SOLDER PAST SOLDER SIDE/POSITIVE			          EXT. GERBER

DRILL_L1_L2.SPL		  		DRILL LAYER ONE TO LAYER TWO  	        EXT. GERBER
DRILL_L1_L4.SPL		  		DRILL THROUGH HOLE (AL LAYERS)          EXT. GERBER
DRILL_L3_L4.SPL		  		DRILL LAYER THREE TO LAYER FOUR         EXT. GERBER
DRILL_L2_L3.SPL		  		DRILL LAYER TWO TO LAYER THREE          EXT. GERBER
                                                                
NCDRILL_L1_L2.SPL 			NC DRILL LAYER ONE TO LAYER TWO		      EXT. GERBER							
NCDRILL_L1_L4.SPL 			NC DRILL THROUGH HOLE (AL LAYERS)       EXT. GERBER
NCDRILL_L3_L4.SPL 			NC DRILL LAYER THREE TO LAYER FOUR      EXT. GERBER
NCDRILL_L2_L3.SPL 			NC DRILL LAYER TWO TO LAYER TREE        EXT. GERBER

batch_to_gerber.rep			DRILL AND NCDRILL REPORT			    ASCII

EXT_GERBER.USR	     		EXTENDED GERBER APERTURE TABLE 		ASCII
CNC.USR		     			    NC DRILL DEVICE FILE	        		ASCII
							
PCB ASSEMBLY FILES:
Z-Accel_PARTLIST_1_2.XLS	PART LIST					EXCEL
P&P_COMP.REP		          PICK AND PLACE COORDINATES, COMPONENT SIDE	ASCII
P&P_SOLD.REP		          PICK AND PLACE COORDINATES, SOLDER SIDE		ASCII
PASTCOMP.SPL         	    SOLDER PASTE COMPONENT SIDE               		EXT. GERBER
PASTSOLD.SPL		          SOLDER PASTE SOLDER SIDE			EXT. GERBER
ASSYCOMP.SPL              ASSEMBLY DRAWING COMPONENT SIDE           		EXT. GERBER
ASSYSOLD.SPL		          ASSEMBLY DRAWING SOLDER SIDE			EXT. GERBER

PDF FILES:
CC2511_USB_Bean_Dongle_SCHEMATIC_1_1.pdf	Circuit Diagram
CC2511_USB_Bean_Dongle_LAYOUT_1_1.pdf		Layout Diagram

CADSTAR FILES:
*.SCM			Cadstar Schematic file
*.CSA			Cadstar Shematic archive
*.PCB			Cadstar layout file
*.CPA			Cadstar PCB archive

README.TXT           	THIS FILE                                 			ASCII

END.
