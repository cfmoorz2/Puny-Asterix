Class MyContainer
    with max_capacity,
        holding [counter i;
            objectloop(i in self)   {
                counter = counter + i.mass;
                !print"counter = ",counter,".^";
                !print"added ",i.mass," to counter.^";
            }
            !print"final counter = ",counter,"^";
            return counter;
        ],
        before [ x;
            receive:
                x = self.holding();
                if (x + noun.mass > self.max_capacity) "There's not enough room in ",(the)self,".";
        ],
    has container;

Class Tiles
    with 
        before [;
            examine:
            if (selected_direction == d_to) "You see scuffed and stained linoleum tiles. ";
        ];

Class Concrete 
    with 
        before [;
            examine:
            if (selected_direction == d_to) "You see concrete below you. ";
        ];

Class DropCeiling
    with 
        before [;
            examine:
            if(selected_direction == u_to) "You see a standard industrial tile drop ceiling. ";
        ];

Class InChair
    with 
        before [ obj;
            take:
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied."; 
            enter:
                if (self == parent(player)) "But you're already sitting in it. ";
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied by ", (the) obj, "."; 
                if (child(self) ~= nothing) "The chair is already occupied.";  
                move player to self;
                if (verb_word == 'stand') "You stand on ", (the) self, "."; else "You settle into ", (the) self, ".";
        ],
    has supporter enterable;

Class OnChair
    with 
        before [ obj;
            take:
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied."; 
            enter:
                if (self == parent(player)) "But you're already sitting on it. ";
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied by ", (the) obj, "."; 
                if (child(self) ~= nothing) "The chair is already occupied.";  
                move player to self;
                if (verb_word == 'stand') "You stand on ", (the) self, "."; else "You sit down on ", (the) self, ".";
        ],
        after [ obj;
            examine:
                objectloop(obj in self && obj has animate) print (The)obj, " is currently sitting in it.^";
        ], 
    has supporter enterable;

Class Bed
    with sit_or_lie,
        before [ ;
            enter:
                if (verb_word == 'sit' or 'lie' or 'get')    {
                    if (self == parent(player) && self.sit_or_lie == verb_word)
                    "You're already on it. ";
                    move player to self;
                    self.sit_or_lie = verb_word;
                    if (AfterRoutines() || keep_silent) rtrue;
                    if (verb_word == 'sit')
                        "You sit on ", (the) self, ".";
                    else    
                        "You lie on ", (the) self, ".";
                }   else    {
                    if (self == parent(player) && self.sit_or_lie == false)
                        "You're already on it. ";
                        move player to self;
                        self.sit_or_lie = false;
                        if (AfterRoutines() || keep_silent) rtrue;
                        "You stand on ", (the) self, ".";
                }

            take:
                "It's quite heavy and you would likely tear something internally. ";
        ],
    has supporter enterable scenery;

Class StairRoom;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object lost;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room morgue "Morgue"
    with name 'morgue',
        description [;
                print "This is the hospital's small morgue. The white tiled floor is scuffed and gently slopes toward a drain in the center of the
                room. The walls are also clad in featureless tiling and a pair of flickering fluorescent tubes above you impart a sickly green 
                ambiance. A stainless steel table sits in the center of the room and one wall is occupied by a bank of refrigerated
                lockers. A door, currently ";
                open_or_closed(morgue_door);
                " lies to the west. ";
            ],
        cheap_scenery  
            3 'white' 'tile' 'tiles//p' "The tiles are cracked and there's some greenish gunk growing in the spaces between them. "
            1 'drain' "It's a standard drain in the floor about three inches across. "
            3 'flickering' 'fluorescent' 'tubes//p' "They're standard fluorescent lights. They emit a sickly green light that 
                makes everyone look corpse-like. "
            3 'refrigerated' 'locker' 'lockers//p' [;
				examine: "You see a bank of six metal doors with metal handles. ";
                open: "You hesitantly open one a crack and peek inside. Luckily, it's empty. ";
				Take, Remove, Turn, Pull, Push: "You decide you don't really need morgue refrigerators. ";
			]
            3'stainless' 'steel' 'table' [;
                examine:
                "It's a narrow stainless steel table that stands about waist level. ";
                take:
                "It's bolted to the floor. ";
                enter:
                "Ew! No. ";
            ],
        w_to morgue_door,
    class Tiles
    has light;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor morgue_door "morgue door" 
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'morgue' && w2 == 'door') return 2;
            if (w1 == 'door') return 1;
        ],
        description [;
            print"It's an unremarkable door, currently ";
            open_or_closed(self);
            ".";
        ],
        npc_open [ npc;
            if (TestScope(npc, player) && self hasnt open) print(The)npc," pushes open the door.^";
            give self open;
            give self ~locked;
        ],
        door_to [;
            if (parent(self) == morgue) return basement_hallway_east; return morgue;
        ],
        door_dir [;
            if (parent(self) == morgue) return w_to; return e_to;
        ],
        found_in morgue basement_hallway_east,
    has scenery door openable open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room basement_hallway_east "Basement Hallway East"
    with description "This is the eastern end of a long dank hallway that continues far to the west. Linoleum tiles cover the floor. A door
                leads east, a placard next to it reads ~Morgue~. An open doorway leads north, A sign above it reads ~Imaging~. 
                Finally, there's an open doorway to the south. A sign over the entrance reads ~M.R.I.~",
        e_to morgue_door,
        w_to elevator_lobby_b,
        n_to x_ray,
        s_to mri_anteroom,
        after [;
            go:
            if (real_location == self && selected_direction == n_to && FlagIsSet(F_TRIO_IS_FOLLOWING))
            {
                trio_discover();
            }
        ]
    class Tiles
    has light; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room mri_anteroom "MRI Anteroom"
    with description "This is the MRI holding area and control room. A fancy glass and metal door leds east to the 
        MRI scanner itself. A large control desk is here. Conveniently, you see two large buttons embedded in the control 
        panel: one green, one red. 
        A computer monitor sits on top of the desk. There's a large red sign on the wall next to the doorway to the 
        scanner. ",
        after [;
            go:
            if (selected_direction == w_to && real_location == self)
            {
                give scanner_door ~open;
                print"With a 'hiss', the door swings closed behind you.^^";
            }
            if (real_location == self && selected_direction == w_to && FlagIsSet(F_TRIO_IS_FOLLOWING))
            {
                trio_discover();
                rtrue;
            }
            if (real_location == self && selected_direction == s_to && FlagIsSet(F_TRIO_IS_FOLLOWING))
            {
                trio.move_mode = 0;
                StopDaemon(trio);
                StartDaemon(anteroom_timer);
            }
        ],
        n_to basement_hallway_east,
        e_to scanner_door
    class Tiles
    has light; 

Object anteroom_timer
    with 
        counter 0,
        daemon [;
            if (self.counter == 4) { trio_catch(); };
            self.counter++;
        ];

Object control_desk "control desk" mri_anteroom
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'control' && w2 == 'panel' or 'desk') return 2;
            if (w1 == 'desk' or 'panel' or 'control') return 1;
        ],
        description "It's a hopelessly complicated control desk covered with knobs and buttons. A monitor 
        is perched on top. Luckily, the important parts seem to be the large green and red buttons embedded in the center. ",
    has supporter scenery container transparent;

Object mri_start_daemon
    with
        count 0,
        daemon [;
            self.count++;
            if (self.count == 4 && (real_location == mri_anteroom || real_location == mri_scanner || real_location == changing_room))
            {
                print"The mechanical whining from within the walls suddenly increases in volume and pitch as the MRI 
                begins to spin up.^^";
            }
            if (self.count == 5)
            {
                mri_handler();
            }
        ];

Object mri_monitor "monitor" mri_anteroom
    with
        name 'monitor' 'crt' 'screen',
        description "It's a boxy amber CRT monitor. ",
        before [;
            take:
            "It seems to be fixed to the control desk. ";
            read:
            if(control_desk.time_left > 0) "It's currently displaying columns of indecipherable text. "; "The screen is currently blank. ";
        ],
    has scenery;

Object green_button "green button" control_desk
    with
        name 'green' 'button',
        description "It's a green button, just asking to be pressed. ",
        before [;
            push:
            if (control_desk.time_left > 0) "Nothing seems to happen. ";
            StartDaemon(mri_start_daemon);
            "You press the button and the monitor suddenly fills with incomprehensible text and numbers. ";
            ],
    has scenery;

Object red_button "red button" control_desk
    with
        name 'red' 'button',
        description "It's a red button. ",
        before [;
            push:
            if (control_desk.time_left > 0)
            {
                StopTimer(control_desk);
                "The monitor screen goes blank and the mechanical whining from the next room spins down to it's 
                previous level. ";
            }
            "Nothing seems to happen. ";
            ],
    has scenery;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor scanner_door "scanner room door" 
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'scanner' or 'mri' or 'glass' or 'metal' && w2 == 'door') return 2;
            if (w1 == 'door') return 1;
        ],
        description"It's a glass and metal door, presumably made from non-magnetic metal. ",
        npc_open [ npc;
            if (TestScope(npc, player) && self hasnt open) print(The)npc," pushes open the door.^";
            give self open;
            give self ~locked;
        ],
        door_to [;
            if (parent(self) == mri_anteroom) return mri_scanner; return mri_anteroom;
        ],
        door_dir [;
            if (parent(self) == mri_anteroom) return e_to; return w_to;
        ],
        found_in mri_anteroom mri_scanner,
    has scenery door openable open ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room mri_scanner "MRI Scanner"
    with
        description [;
            print "This nearly empty room is dominated by the MRI scanner occupying the center of it. The scanner 
            itself is a large boxy structure with a narrow round opening in the side. A narrow padded bed is connected
            to the scanner. It sits on a platform and allows the bed to slide in and out of the bore of the scanner. A 
            tangle of pipes and conduits exits the machine and traverses the walls and ceiling. There's a narrow open doorway
            leading east and the doorway back to the control room lies to the west. A large red warning sign is posted
            next to the exit.^";
            if (self.room_is_trashed == true) 
            {
                print"Currently, the room is in shambles. Metal oxygen cannisters litter the floor and there are gaping holes 
                in the walls and in the MRI machine itself. Sparks fly from the gashes in the electrical conduits lining 
                the walls.^^";
            }
            if (injured_trio in self)
            {
                print"Nurse Retch, Vic, and Dr. Northrup are here groaning in pain on the ground.^";
            }
            if(letter in self && letter.fluttering == 1)
            {
                print"^A letter falls from Northrup's pocket and flutters to the floor.^";
            }
        ],
        describe [; rtrue; ],
        before [;
            go:
            if (selected_direction == w_to && trio in self)
            {
                "The angry trio blocks your exit.^";
            }
        ],
        w_to scanner_door,
        e_to changing_room,
        room_is_trashed false,
        after [;
            go:
            if (selected_direction == e_to && real_location == self)
            {
                give scanner_door ~open;
                print"With a 'hiss', the door swings closed behind you.^^";
            }
            if (selected_direction == e_to && real_location == self && FlagIsSet(F_TRIO_IS_FOLLOWING))
            {
                !StopDaemon(trio);
                StopDaemon(anteroom_timer);
                StopDaemon(trio_contact_daemon);
                trio.move_mode = 0;
                move trio to self;
                StartDaemon(final_daemon);
                "The door opens after you and the angry trio of Northrup, Vic, and Retch pile into the room. 
                Northrup is breathing hard and red-faced. He straightens his white coat and runs his hand through 
                his hair in an effort to recover his dignity.^^
                ~This ends now, my dear. We can't have any loose ends, you know.~";
            }
        ],
    class Tiles
    has light;  

Object final_daemon 
    with 
        count 0,
        daemon [;
            switch(self.count)
            {
                4:
                    trio_final();
                    rtrue;
                1:
                    print"Vic lets out an evil chuckle.^";
                2:
                    print"Nurse Retch stares at you through crazy eyes.^";
            }   
            self.count++;
        ];

Object mri_machine "MRI machine" mri_scanner
    with
        name 'mri' 'machine' 'scanner',
        description [;
            print"It's a large boxy machine with a round opening in the side and a narrow padded platform at the mouth. ";
            if(mri_scanner.room_is_trashed) "Currently, it's a smoking hulk of wreckage with holes punched in it and
            sparks flying from it. "; "";
        ],
        before [;
            enter:
            "You climb partway into the narrow open in the MRI. You're immediately hit with a wave of claustrophobia and you climb back out. ";
        ],
    has scenery container open transparent;

Object mri_platform "mri table" mri_scanner
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'mri' or 'scanner' && w2 == 'table' or 'platform' or 'bed') return 2;
            if (w1 == 'table' or 'platform' or 'bed') return 1;
        ],
        description "It's a narrow padded table that the patient lies on while they slide in and out of the MRI scanner. ",
    has scenery supporter enterable;  

Object mri_warning_sign "warning sign" 
    with
        name 'sign' 'warning',
        description "It's a large sign, quite insistent, with large red lettering and lots of exclamation points. ",
        before [;
            read:
            "~Warning! Strong magnetic field! This magnet is always on! No loose metal objects! Objects made from or containing 
            non-ferrous metals should not be taken into this room!~";
        ],
        found_in mri_scanner mri_anteroom,
    has scenery; 

Object holes "holes" 
    with 
        name 'gash' 'gashes' 'holes' 'hole',
        description "There are large holes punched out of the walls and the machinery in the room. Sparks fly from exposed 
        wiring and there's a haze of acrid smoke in the air. ",
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room changing_room "Changing Room" 
    with 
        description "This small dark alcove serves as a changing space for those robing or disrobing for their 
        MRI scan. There's a small wooden bench built into the wall but the room is otherwise empty. 
        The only way out is to the west. ",
        w_to mri_scanner,
        out_to mri_scanner,
    class Tiles
    has light;  

OnChair bench "bench" changing_room
    with 
        name 'bench' 'platform',
        description "It's a small wooden bench built into the wall.  ",
    has scenery;    

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_b "Basement Elevator Lobby" 
    with description [; 
        print"This is a spartan fluorescent-lit hallway that continues to the east and west.  A stairwell lies to the north through an 
        open doorway. Opposite the stairwell in the south wall stands the main elevator. Next to it you see a sign reading ~Morgue, 
        Imaging~ above an arrow pointing east and one reading ~Central Supply~ above one pointing west. The elevator doors are currently ";
        open_or_closed(elevator_doors);
        ". The call button is embedded in a small panel next to the door.";
        ],
        e_to basement_hallway_east,
        n_to stairwell_b,
        s_to elevator_doors,
        in_to elevator_doors,
        w_to basement_hallway_west,
    class Tiles DropCeiling
    has light,
    ;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_b "Basement Stairwell" 
    with description
        "This is a dark stairwell just off the main hallway which lies to the south. A large ~B~ is painted on the wall. 
        Concrete stairs zig-zag upward to the floor above.",
        s_to elevator_lobby_b,
        u_to stairwell_m,
    class Concrete StairRoom
    has light,
    ;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room basement_hallway_west "Basement Hallway @@64 Service Elevator"
    with description
        "This is nearly the midpoint of a long dreary east-west hallway. A service elevator is here to the north. A sign next to it 
        reads ~Employees Only~ and a small panel is embedded in the wall next to it. ",
        e_to elevator_lobby_b,
        w_to hallway_b1,
        n_to service_elevator_door,
        in_to service_elevator_door,
    class Tiles
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_b1 "Basement Hallway @@64 Dumbwaiter"
    with description "The corridor here continues east and west. Embedded in the wall to the south you see an 
    old dumbwaiter, presumably once used to deliver needed items to the patient floors above.",
    e_to basement_hallway_west,
    w_to hallway_b2,
    class Tiles
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_b2 "Basement Hallway West"
    with description "This is the western end of a long corridor that stretches to the east under flickering fluorescent lights. 
        There's an open doorway to the north, a sign next to it reads ~Environmental Services~.
        A second doorway lies to the west. A sign next to it reads ~Engineering~. Finally, a substantial metal door lies to the 
        south, a placard on it reads ~Central Supply~.",
        e_to hallway_b1,
        n_to environmental_services,
        s_to central_supply_door,
        w_to engineering,
    class Tiles 
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room central_supply "Central Supply" 
    with description "This is a large open space crowded with dusty crates and defunct antique equipment. There are scattered low-wattage lamps 
    hanging from the ceiling but most of these have long ago burned out, leaving the room quite dark with grotesque shadows 
    projected onto the concrete walls. An impossible tangle of pipes and conduits hangs from above. The only exit lies through a metal door to the
    north.",
    n_to central_supply_door,
    cheap_scenery
    5 'pipes' 'tangle' 'conduit' 'pipe' 'conduits' "It's a dark labyrinth of pipes of all kinds running along the ceiling. "
    2 'shadow' 'shadows' "Ooh. Scary. "
    2 'lamp' 'lamps' "They're simple green lamps hanging from the ceiling above. "
    3 'crates' 'antique' 'equipment' "It appears to be ancient artifacts from the the dark past of medical history. ",
    before [;
        examine:
        if(selected_direction == u_to) "You see exposed ducts and piping. ";
    ],
    class Tiles
    has light;

Object oxygen_cart "oxygen cart" central_supply
    with 
        name 'metal' 'wheeled' 'cart',
        article "an",
        description"It's a metal wheeled cart. It's loaded with cannisters of oxygen. ",
        mass 100,
        before [ dirobj x;
            take:
                "It's far too heavy. ";
            receive:
                "The cart is already full. ";
            push:
                "You should supply a direction. ";
            pushdir:
                dirobj = DirPropToFakeObj(selected_direction);
                if (real_location.selected_direction == 0) "You can't go that way. ";
                if (dirobj ==  FAKE_U_OBJ or FAKE_D_OBJ) "You can't push it up or down stairs. ";
                print"Wheels grinding and squeaking, you push the cart ";
                !print"Wheels grinding and squeaking, you push the cart to the ";
                x = selected_direction_index;
                if (x == 7) print"inside";
                if (x == 8) print"out";
                if(x ~= 7 && x ~= 8)
                {
                    !print (string) direction_name_array-->selected_direction_index; print" has index ",x,"^";
                    print (string) direction_name_array-->selected_direction_index;
                }
                print".^^";
			    <Go dirobj>;
			    move self to real_location;
                rtrue; 
            pulldir:
                dirobj = DirPropToFakeObj(selected_direction);
                if (real_location.selected_direction == 0) "You can't go that way. ";
                if (dirobj ==  FAKE_U_OBJ or FAKE_D_OBJ) "You can't pull it up or down stairs. ";
                print"Wheels grinding and squeaking, you pull the cart ";
                !print"Wheels grinding and squeaking, you pull the cart to the ";
                x = selected_direction_index;
                if (x == 7) print"inside";
                if (x == 8) print"out";
                if(x ~= 7 && x ~= 8)
                {
                    !print (string) direction_name_array-->selected_direction_index; print" has index ",x,"^";
                    print (string) direction_name_array-->selected_direction_index;
                }
                print".^^";
			    <Go dirobj>;
			    move self to real_location;
                rtrue;    
        ],
    has supporter;

    Object oxygen_cannisters "oxygen cannisters" oxygen_cart
        with 
            parse_name [ w1 w2;
                w1 = NextWord();
                w2 = NextWord();
                if (w1 == 'oxygen' && w2 == 'cannister' or 'cannisters') return 2;
                if (w1 == 'oxygen' or 'cannister' or 'cannisters') return 1;
            ],
            description [;
                print"They're large metal oxygen cannisters. ";
                if (self in oxygen_cart) { print"They're stacked neatly on a metal cart. ";}  else { 
                    print"They're haphazzardly scattered about the room. "; }
                "Each one is labeled with a warning: Not for use in the vicinity of MRI machines. ";
            ],
            before [;
                take:
                "Each cannister is too heavy and unwieldy. ";
            ] ,
        has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor central_supply_door "door" 
    with name 'metal' 'door',
        description [;
            print"It's a thick metal door, currently ";
            open_or_closed(self);
            print ".";
        ],
        door_to [;
            if (parent(self) == hallway_b2) return central_supply; return hallway_b2;
        ],
        door_dir [;
            if (parent(self) == hallway_b2) return s_to; return n_to;
        ],
        found_in central_supply hallway_b2,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room environmental_services "Environmental Services"
    with description [; 
        print"This is a cluttered storage room. The walls are lined with shelves stocked with 
        cleaning supplies, brushes, and brooms, none of which you need to concern yourself with. 
        A tall but narrow brown metal cabinet stands in one corner and a desk is pushed up against one wall. ";
        "A black telephone is mounted on the wall. The exit lies to the south. ";
    ],
        cheap_scenery
        4 'containers' 'solvents' 'container' 'solvent' [;
            examine: 
            "Most of them display a little skull-and-crossbone on the side. ";
            take: 
            "Ew, no. You are definitely not interested in cleaning anything. ";
        ]
        4 'brush' 'broom' 'brushes' 'brooms' [;
            examine:
            "They're neatly organized by size and type. ";
            take:
            "Ew. You don't need a dirty broom. ";
        ]
        1 'dial' [;
            examine:
            "It's a plastic rotary phone dial. ";
            Take:
                "It's part of the handset. ";
        ]
        1 'cord' [;
            examine:
            "It's a black coil of phone cord. ";
            Take:
            "That's attached to the phone.";
        ],
        s_to hallway_b2,
    class Tiles
    has light;

Object environmental_desk "desk" environmental_services
    with name 'desk' 'wooden',
        description [;
        print "It's a small wooden desk, quite stained. ";
        if(PrintContents("On the desk you can see ", self)) "."; else "";
        ],
    has supporter scenery;

Object storage_cabinet "cabinet" environmental_services
    with 
        id, ! 0 = cabinet, 1 = door
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'cabinet' && w2 == 'door') { self.id = 1; return 2; }
            if (w1 == 'cabinet' or 'brown' or 'metal' or 'tall' or 'narrow' or 'storage') { self.id = 0; return 1; }
            if (w1 == 'door') { self.id = 1; return 1; }
        ],
        description [ ;
            if(self.id == 0)
            {
                print"It's a tall brown metal storage cabinet, currently ";
                if(self has open) "open. "; else "closed. ";
            }
            print"It's the metal door to a storage cabinet, currently ";
            if(self has open) "open. "; else "closed. ";
        ],
        before [;
            open:
            if (self has open) "It's already open. ";
            give self open;
            print"You open it to reveal a bunch of useless junk";
            if (kcl_bottle in self) " and an out-of-place appearing plastic vial. ";
                " and not much else. ";
        ],
    has scenery container openable ~open;

Object useless_junk "useless junk" storage_cabinet
    with
        name 'useless' 'junk',
        description "It's an assortment of thoroughly ignorable junk. ",
    has pluralname static;

Object telephone "telephone" environmental_services
    with name 'black' 'phone' 'telephone',
        description [;
            print"It's a black push-button phone. The base is mounted to the wall. ";
            if (handset in self) "The handset is in the cradle. "; else "";
            ],
        mass 0,
        before [;
            DialObj:
                <<dialobj handset>>;
            DialNumber:
                <<dialnumber handset>>;
            Take:
                <<take handset>>;
            Drop:
                <<drop handset>>;
            receive:
            "You can't put things in the phone. ";
        ],  
        describe [;
            rtrue;
        ],
        invent [ ;
            if (inventory_stage == 2) rtrue;
        ],
    has scenery container open transparent;

Object handset "handset" telephone 
    with name 'handset',
        description "It's a black phone handset. It has a rotary dial and is attached to the wall unit by a black coiled cord. ",
        mass 0,
        before [;
            DialObj:
                if (self notin player) print "(first taking the handset)^";
                DialPhone();
                move self to telephone;
                "^You hang up the phone. "; 
            Drop:
                if (self in player) {
                    move self to telephone;
                    "You hang up the phone. ";
                }   else "You're not holding the handset. "; 
        ],
        after [;
            Take:
                "You hold it to your ear and hear a dial tone. ";
        ],
        describe [;
            rtrue;
        ],
        react_before [;
            Go:
                if (self in player)   {
                    move self to telephone;
                    print"(first hanging up the phone)^";
                };
            DialNumber:
                if (self notin player) print "(first taking the handset)^";
                if (noun == 911 || noun == 000)
                {
                    print"The phone rings and a bored female voice answers. ~";
                    if(noun == 911) { print"911"; } else { print"000"; }
                    print", what is your emergency?~ Suddenly there's a burst of static and call is disconnected. Damn blizzard.^";
                    move self to telephone;
                    "^You hang up the phone. ";
                }  
                DialPhone();
                move self to telephone;
                "^You hang up the phone. ";  
        ],
    has concealed;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room engineering "Engineering"
    with description "This is a small cramped room that smells like lightning. Thick bundles of 
        electrical conduits descend like a massive pipe organ from 
        the ceiling into a large fuse box on the wall. The only exit is east through a plain metal door. ",
        cheap_scenery
        2 'conduit' 'conduits' "You see tangles and bundles of wires leading into a fuse box. ",
        before [;
            Smell:
                "It smells mechanical in here, with a hint of electricity. ";
            examine:
            if(selected_direction == u_to) "You see exposed ducts and electrical conduits. ";
        ],
    e_to hallway_b2,
    class Tiles
    has light;

Object fuse_box "fuse box" engineering 
    with 
        name 'fuse' 'box' 'fuses',
        description [;
            print"It's a large gray metal box. Wires and electrical conduits sprout from it and climb upward to the ceiling. 
            A plaque on the front reads ~WARNING - HIGH VOLTAGE~. ";
            if(self has open)
            {
                print"It's currently open, revealing a tangle of switches and connectors. ";
                if (plastic in engineering)   "There's a small round piece of plastic wedged between two of the contacts."; "";
            }
            "";
        ],
        before [;
            search:
            if (self hasnt open) "The fuse box is closed. ";
            print"The fuse box is full of switches and connectors. ";
            if (plastic in engineering) "There's a round piece of plastic jammed between two of the connectors. "; "";
            receive:
            "You shouldn't poke around in high-voltage fuse box. ";
            take:
            "Rubbish.";
            open:
            if (self has open) "It's already open. ";
            give self open;
            plastic.noticed = true;
            print"You open it to reveal a confusing jumble of switches and connectors. ";
            if (plastic hasnt moved) "There's a round piece of plastic jammed between two of the connectors. "; "";
        ],
    has scenery container openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_m "Main Stairwell" 
    with description 
        "This is a dark stairwell just off the main hallway which lies to the south. The word ~Main~ is painted in large letters 
        on the wall here. Concrete stairs zig-zag up and down to 
        the floors above and below.",
    d_to stairwell_b,
    s_to elevator_lobby_m,
    u_to stairwell_2,
    class Concrete StairRoom
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_2 "Second Floor Stairwell"
    with description "This is a dark stairwell just off the main hallway which lies to the south. 
        A large number ~2~ is painted on the wall. Concrete stairs zig-zag downward to the floor below. ",
        d_to stairwell_m,
        s_to elevator_lobby_2,
    class Concrete StairRoom
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room b_ward_1 "Ward B - West"
    with description "The hallway continues east and west. Here the walls are blue to match the line running 
        down the middle of the floor. Patient rooms lie north and south through open doorways. A sign next to the former 
        reads ~31~, one next to the latter reads ~32~.", 
        name 'ward' 'west',
        w_to hallway_2_3,
        e_to station_b,
        n_to room_31,
        s_to room_32,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_33 "Room 33"
    with description "This is an unassuming patient room. The walls are blue and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to station_b,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ],
    class Tiles DropCeiling
    has light;

Bed room_33_bed "hospital bed" room_33
    with name 'hospital' 'bed',
        description "It's a standard hospital bed. ",
        before [;
            enter:
                "Larry is fully occupying the bed. ";
        ]; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_34 "Room 34"
    with description "This is a single-occupancy patient room. The walls are blue and a single bed is pushed against the wall. 
        There's a doorway leading out to the north. ",
        n_to station_b,
    class Tiles DropCeiling
    has light;

Bed generic_bed "hospital bed" 
    with name 'hospital' 'bed',
        description "It's a standard hospital bed. ",
        before [;
            enter:
                "It's not a sleepover. ";
        ],
        found_in room_34 room_31 room_24 room_32;


!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_32 "Room 32"
    with description "This is a single-occupancy room. The walls are blue and a single bed is pushed against the wall.
        The floor is scuffed linoleum and a steaming radiator hisses under a window. A bed is pushed against one 
        wall and the exit is to the north. ",
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ]
        1 'pillow' [;
            examine:
            "It's a thin lumpy pillow. ";
            take:
            "You don't need the lumpy pillow. ";
        ]
        'green' 'blanket' [;
            examine:
            "It's a thin greenish blanket. ";
            take:
            "Edith needs the blanket more than you do. ";
        ],
        n_to b_ward_1,
    class Tiles DropCeiling
    has light;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room station_b "Ward B - East"
    with description "This is the Ward 'B' nurses' station, lying at the eastern end of a long east-west corridor. To aid in 
        visitor and patient navigation, the walls here are painted blue and a blue line runs to the west down the 
        middle of the hall towards the main elevators. Open doorways lead to patient rooms to the north and south. 
        A sign next to the former reads ~33~, next to the latter ~34~. A large wide waist-high desk occupies the center of the room,
        allowing space for medical staff to chart and dictate. ",
        w_to b_ward_1,
        n_to room_33,
        s_to room_34,
    class Tiles DropCeiling
    has light;

Object ward_b_station "nurses' station" station_b
    with name 'nurse' 'station' 'desk',
        description "It's a tall standing desk, square and roughly waist high. It gives the nurses and physicians 
        a surface to update notes and charting. ",
        has supporter scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_31 "Room 31"
    with description "This is an unassuming patient room. The wallpaper is an institutional blue and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to b_ward_1,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ],
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_m "Main Elevator Lobby"
    with description [;
        print"This is the main elevator lobby. The elevator doors, currently ";
        open_or_closed(elevator_doors); 
        ", lie to the south, the 'up' and 'down' buttons 
        are embedded in a small panel next to them. An open doorway leads into a stairwell to the north and an open double 
        doorway leads to the east. A sign across the top reads ~Cafeteria~. A long corridor begins here and leads west. ";
        ],
        cheap_scenery
            'cafeteria' 'sign' "It's a large sign over the doorway that reads ~Cafeteria~. ",
        n_to stairwell_m,
        e_to cafeteria,
        w_to hallway_m1,
        s_to elevator_doors,
        in_to elevator_doors,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room cafeteria "Cafeteria"
    with description "This is the hospital cafeteria, dark and empty this time of night. Tables are arranged in rows, inverted chairs 
        stacked on top of them. A long stainless steel buffet serving station lines one wall, an abandoned cashier
        station against another. An open doorway leads north into the kitchen and the exit to the main hallway lies to the west. ",
    w_to elevator_lobby_m,
        cheap_scenery
            'table' 'tables//p' [;
                examine:
                "They're standard cafeteria tables. Just like the ones you sit at in study hall. ";
                Take:
                "I don't think so. ";
                Enter:
                "You don't need to climb around on the tables. ";
                Receive:
                "The tables are already occupied by inverted chairs. ";
            ]
            'chair' 'chairs//p' [;
                examine:
                "They're beat up vinyl cafeteria chairs. ";
                take:
                if (cafeteria_chair in self) rfalse; "You've already taken one of the chairs. ";
            ]
            5 'steam' 'serving' 'table' 'buffet' 'station' [;
                take:
                "Seems unrealistic. ";
                examine: 
                "It's a long aluminum industrial steam table. The wells are all empty. ";
                search:
                "There's nothing hidden in the buffet station. ";
        ],
    n_to kitchen,
    class Tiles DropCeiling
    has light;

OnChair cafeteria_chair "vinyl chair" cafeteria
    with name 'orange' 'vinyl' 'chair',
        description "It's a rather beat up cafeteria chair molded from dirty orange vinyl. ",
        describe [; if(self in cafeteria) rtrue; ],
        before [;
            take:
                if(self hasnt moved) print"You pick one of the chairs at random.^";
            enter:
                if(self hasnt moved) print"You pick one of the chairs at random.^";
        ],            
        mass 15;

Object cashier_station "cashier station" cafeteria 
    with name 'cashier' 'station',
        description "It's a metal stand holding an old and very scratched metal cash register. ",
    has scenery;

Object cash_register "cash register" cafeteria 
    with name 'cash' 'register', 
        description [;
            print"It's a very old and weathered metal cash register fixed to the cashier stand. 
            There aren't very many buttons and the numbers on them have largely worn off. ";
            if (cash_drawer has open) "The drawer is currently open. "; else "The drawer is currently closed. "; 
        ],
        describe [;
                    rtrue;
        ],
        before [;
            Take:
                "It seems to be bolted down, likely with sticky-fingered people like yourself in mind. ";
            Insert:
                <<insert noun cash_drawer>>;
            Open:
                <<open cash_drawer>>;
            Close:
                <<close cash_drawer>>;
        ],
    has scenery container open;

Object cash_drawer "cash drawer" cafeteria
    with name 'cash' 'drawer' 'till',
        description [;
            if (self has open) "The till is currently open. "; else "The till is currently closed. ";
        ],
        before [;
            push:
            <<close self>>;
        ],
        after [;
            close:
            give self locked;
        ],
        max_capacity 6,
    class MyContainer
    has open openable scenery;

Object cash_buttons "buttons" cafeteria 
    with name 'button' 'buttons//p',
        description "They're large mechanical cash register buttons. Whatever labels they once had have largely worn away. ",
        before [;
            push:
                "You randomly poke at the buttons, but nothing useful seems to happen. ";
            take:
                "They're attached to the register. ";
        ],
    has scenery pluralname;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room kitchen "Kitchen"
    with description "Stainless steel prep tables occupy the center of this large room with ovens and stoves arranged along the walls.
    The overhead fluorescent lights are off and it's nearly pitch dark in here, save for the faint green light coming from an 'exit' sign
    over the door to the south. There's a garbage can sitting next to one of the prep tables. ",
    cheap_scenery 
        4 'steel' 'prep' 'tables//p' 'table' [;
            Take:
            "They seem to be bolted to the floor. ";
            Enter:
            "They've been sanitized and you would sully them. ";
            Receive:
            "The tables are very clean and only intended for food products. ";
            examine:
            "They're stainless steel tables occupying the center of the room. ";
        ]
        6 'oven' 'ovens//p' 'stove' 'stoves//p' 'appliance' 'appliances//p' [;
            take:
            "You would need a crew of movers. ";
            enter:
            "You don't need to crawl around in or on that. ";
            receive:
            "You can leave that to the cooks. ";
            examine:
            "They're old but clean industrial appliances arranged around the periphery of the kitchen. ";
        ]
        3 'fluorescent' 'light' 'lights//p' [;
            take:
            "You can't reach them and don't need them. ";
            examine:
            "They're bare fluorescent tubes. They're all off. ";
        ]
        3 'exit' 'light' 'sign' [;
            take:
            "You can't reach it and don't need it. ";
            examine:
            "It's a glowing green 'exit' sign. ";
        ],
    s_to cafeteria,
    class Tiles DropCeiling
    has light;

Object garbage_can "garbage can" kitchen
    with name 'garbage' 'rubbish' 'can' 'bin',
        description "It's a large metal garbage can. ",
        before [;
            take:
                "It's too heavy. ";
            search:
                if(shrimp hasnt moved) move shrimp to self;
        ],
        max_capacity 20,
    class MyContainer
    has scenery open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_m1 "Junction"
    with description "Two corridors meet here. A dark wide hallway continues east and west and a short wood-panelled 
        corridor begins here and leads south. ",
        s_to main_lobby,
        w_to hallway_m2,
        e_to elevator_lobby_m,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_m2 "Main Hallway @@64 Dumbwaiter"
    with description "The hallway here continues east and west. An old dumbwaiter is embedded in the south wall. 
    You notice that one of the tiles in the drop ceiling above is ajar and you can see darkness behind it. ",
        before [;
            go:
            if (selected_direction == u_to && ladder in self) 
                if (ladder hasnt open) { "The ladder is folded up. "; }
                else
                {
                print"You climb the creaky metal ladder nearly to the top. Once there, you are able to 
                grab onto some sturdy pipes that you see through the gap in the tiling and pull yourself into the ceiling above. 
                Breathing heavily but victorious, you think back to P.E. class freshman year when you couldn't climb a knotted 
                rope in gym class and Becky Harris laughed at you. Eat it Becky!^^";
                PlayerTo(ceiling_01);
                rtrue;
                }
            if (selected_direction == u_to && (player in cafeteria_chair || player in security_chair || player in break_room_chair))
                "Standing on the chair doesn't get you nearly high enough to reach. ";
            examine:
            if (selected_direction == u_to) "You notice one of the large tiles in the drop-ceiling is slightly out of alignment. 
                You can see darkness behind it. ";
            if (selected_direction == d_to) "You see a fairly nice dark blue carpet. ";
        ],
        after [;
            go:
            if (real_location == self && selected_direction == e_to && ledger in player)
            {
                move trio to admin_hallway;
                StartTimer(trio_follow_timer, 2);
                StartDaemon(trio_contact_daemon);
                if(real_location == hallway_m1 || real_location == hallway_m2)
                    print"Suddenly, in the shadows down the corridor to the west you see a vague shape. In a moment you 
                    see the trio of Dr. Northrup, Nurse Retch, and Vic step out of the gloom. ";
                    if (ledger in player) "They don't look happy and their eyes are fixed on the ledger you're holding. "; ""; 
            }
        ],
        cheap_scenery
        2 'tile' 'tiles' [ ;
            examine:
            "One of the tiles in the drop ceiling is askew. There's dark empty space above it. ";
            take: 
            "You can't reach it and don't need it. ";
        ]
        1 'ceiling' "It's a drop-ceiling. You notice one of the tiles is askew. ",
    e_to hallway_m1,
    w_to admin_hallway,
    class Tiles DropCeiling
    has light;



Object ajar_tile "tile" hallway_m2
    with 
        name 'tile' 'tiles' 'ajar',
        description "It's a tile in the drop ceiling that's knocked aside. ",
        before [;
            take:
            "You can't reach it from here. ";
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room security_office "Security Office" 
    with description [;
            print"This cluttered office smells vaguely of old pizza and sour-cream potato chips. A bank of 
            closed-circuit monitors lines one wall, although none of them are turned on or seem to be functional. A long desk
            fronts the monitors. It's ringed with water stains and burger wrappers. ";
            print"The exit lies to the east.^";
            if (buzz in security_chair) {
                if (buzz hasnt encountered) {
                    print"^Buzz, the head of security, is sitting at the desk. He's engrossed in the newspaper and 
                        intermittently reaches over and blindly plucks a shrimp from a bowl and plops it into his mouth.^"; 
                    give buzz encountered;
                    ActivateTopic(mabel, 303);
                    ActivateTopic(freddy, 301);
                    StartDaemon(buzz);
                } else {
                    "^Buzz is sitting here, reading the newspaper. ";
                }
            }
        ],
        cheap_scenery 
            5 'water' 'rings//p' 'ring' 'stain' 'stains//p' "They're water stains, likely permanent. Use a coaster, people. "
            4 'burger' 'wrapper' 'wrappers//p' 'garbage' [;
                take:
                "It's literally garbage. ";
                examine:
                "They're greasy wadded up paper wrappers. ";
            ]
            8 'closed' 'circuit' 'monitor' 'monitors//p' 'television' 'televisions//p' 'tv' 'tvs//p' [;
                take:
                "They're fixed in place and quite heavy. ";
                examine:
                "It's a low bank of closed-circuit monitors. None of them appear to work. ";
            ],
        before    [;
            smell:
                if (noun == 0) "It smells like onions, garlic, and old cheese. ";
        ],
        e_to security_door,
    class Tiles DropCeiling
    has light;

Object security_desk "security desk" security_office
    with name 'security' 'desk',
        description"It's a long desk facing the bank of closed-circuit monitors. ",
    has supporter scenery;

OnChair security_chair "metal chair" security_office
    with name 'metal' 'chair',
        description "It's a standard metal and plastic chair. ",
        mass 25,
        describe [;
            if (self in security_office)
                rtrue;
        ];

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor security_door "security door"
     with name 'metal' 'security' 'door',
        description [;
            print"It's an unassuming metal door, currently ";open_or_closed(self);".";
        ],
        door_to [;
            if (parent(self) == main_lobby) return security_office; return main_lobby;
        ],
        door_dir [;
            if (parent(self) == main_lobby) return w_to; return e_to;
        ],
        found_in main_lobby security_office,
    has scenery door openable open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room main_lobby "Main Lobby"
    with description [;
        if(FlagIsClear(F_ENDGAME))
            "This is the main lobby and hospital entrance. The wall to the south is mostly glass, including the large 
            glass double doors leading out. The windows are partially iced over and through them you can see gales of 
            wind-driven snow blowing sideways. An information desk occupies the north wall and the door to the security
            office lies to the west. ";
            "This is the main lobby and hospital entrance. The wall to the south is mostly glass, including the large 
            glass double doors leading out. The snow has stopped and 
            faint pink sunlight illuminates the frosted windows from behind. An information desk occupies the north wall
            and the door to the security office lies to the west. ";
        ],
        cheap_scenery
        'information' 'desk' "It's a round wooden desk with the word 'Information' emblazoned across the front. "
        'windows//p' 'window' "They're icing over and difficult to see through. Through them you can 
            intermittently catch a glimpse of the blizzard outside. "
        3 'blizzard' 'storm' 'snow' "It's really blowing out there. Through the iced-over windows you catch an occasional glimpse 
            of ice and snow blowing sideways. ",
        n_to hallway_m1,
        w_to security_door,
        before [;
            go:
                if (selected_direction == s_to or out_to)
                "The main entrance doors seem to have been locked for the night. ";
            ],
    class Tiles DropCeiling
    has light;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor main_lobby_doors "lobby doors" main_lobby
     with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'lobby' or 'double' or 'glass' && w2 == 'doors') return 2;
            !if (w1 == 'lobby') return 1;
            if (w1 == 'doors') return 1;
            if (w1 == 'entrance') return 1;
        ],
        description "They're a pair of glass doors. ",
    has scenery door openable locked pluralname;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room admin_hallway "Administration"
    with description [;
        print"This is the western end of a long east-west hallway. It's a bit better maintained 
        than the others and warmer colors accentuate the walls. The overhead fluorescent tubes are replaced 
        here by round light fixtures and the floor is covered with blue carpet. There's a plain wooden door 
        to the north and a fancy mahogany door to the south. Each has a small brass placard mounted on it. ";
        if (FlagIsClear(northrup in northrup_office) && northrup_door hasnt open) "There's a crack of light under the door 
        to the south. "; else ""; 
    ],
    cheap_scenery
    4 'light' 'lights' 'fixtures' 'fixture' "They're brass light fixtures hanging from the ceiling. "
    'blue' 'carpet' "It's dark blue carpet. It looks relatively new. "
    4 'placard' 'placards' 'sign' 'signs' "They're small brass signs on the doors. ",
    before [;
        examine:
        if (selected_direction == u_to) "You see a tile drop ceiling and light fixtures hanging down. ";
        if (selected_direction == d_to) "You see blue carpeting. ";
    ],
    e_to hallway_m2,
    n_to jorry_door,
    s_to northrup_door,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor jorry_door "plain wooden door" 
    with name 'door' 'wooden' 'plain' 'north',
        description [;
            print"It's a plain wooden office door, currently ";
            open_or_closed(self);
            if (real_location == admin_hallway) ". A brass placard on the door reads: ~Sid Jorry, VP/CFO~"; "";
        ],
        door_to [;
            if (parent(self) == admin_hallway) return jorry_office; return admin_hallway;
        ],
        door_dir [;
            if (parent(self) == admin_hallway) return n_to; return s_to;
        ],
        with_key dull_key,
        found_in admin_hallway jorry_office,
    has scenery door openable ~open lockable locked;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room jorry_office "Jorry's Office" 
    with description [;
        print"This is a typical office befitting a less-than-top-level executive. A desk stands here 
        as well as a rolling leather executive chair";
        print". Non-threatening paintings of flowers and bowls of fruit adorn the walls. 
        A squat black safe sits in one corner of the room. and a large glass aquarium sits on 
        a stand directly behind the desk. Sand covers the bottom and a snake is coiled around a bleached twisted 
        branch at one end. 
        ^^The former Sid Jorry is sitting in the chair and slumped over the desk.
        His eyes, thankfully, are closed.^";
        if (self hasnt visited) "^It appears that someone has been in here already and cleaned out the room. 
            There's no trace of any papers, documents, or your letter of completion. ";
    ],
        before [;
            examine:
            if(selected_direction == u_to) "You see a tile drop ceiling. ";
            if(selected_direction == d_to) "You see blue carpet. ";
        ],
    cheap_scenery
    1 'sand' "You don't need the sand. Plus it gets everywhere..."
    3 'painting' 'paintings' 'picture' [;
        examine:
        "They're generic prints of flowers and fruit. You feel both soothed and insulted just looking at them. ";
        take:
        "You have no need for mediocre art. ";
    ]
    3 'combination' 'lock' 'dial' [;
        examine:"It's a dial with the numbers 1 through 35 around the periphery. A large hole has been drilled in it
            rendering it useless. ";
        turn:
            "It's broken and won't turn. ";
    ]
    3 'wrought' 'iron' 'stand' "It's a wrought iron stand holding up the aquarium. "
    3 'bleached' 'branch' 'stick' [;
        examine:
        "It's a tortuous piece of bleached wood. There's currently a snake wrapped around and sleeping on it. ";
        take:
        "Not with the snake there, nosirree. ";
    ]
    4 'chair' 'seat' 'rolling' 'leather' [;
        examine:
        "It's a faux-leather rolling executive chair. ";
        take:
        "It's too heavy what with the dead body in it. ";
        enter:
        "There's currently a deceased administrator in it. ";
    ],
    each_turn [;
        print (string) random("^Louanne, curious, flicks her forked tongue at you.^",
            "^Louanne stares at you through yellow reptilian eyes.^", "", "");
    ],
    s_to jorry_door,
    has light;

Object jorry_safe "safe" jorry_office
    with 
        name 'safe',
        description "It's a black squat office safe. The combination lock has been drilled out and the door is bent. ",
        before [;
            take:
            "It's far too heavy. ";

            close:
            "The door is bent and it won't close. ";
        ],
        max_capacity 10,
    class MyContainer
    has scenery open;

Object jorry_desk "desk" jorry_office
    with 
        name 'desk',
        description "It's a standard executive desk, wooden with a glass top. A dead body is currently sprawled across it.",
    has scenery; 

Object aquarium "aquarium" jorry_office
    with name 'aquarium' 'tank',
        description [;
            print"It's a large aquarium with a plastic cover over the top. A thick layer of sand lines the bottom.
            A twisty bleached piece of branch sits in one end, a large snake is coiled motionless on and around it. ";
            if (rock in self) "A large decorative rock sits in the sand in the other end. "; "";
        ],
        before [;
            receive:
            "Louanne would politely request that you not litter her tank. ";
        ],
    has scenery transparent container openable;

Object louanne "Louanne" aquarium 
    with 
        name 'python' 'snake' 'louanne',
        description "She's a two-foot long reticulated python although you have no way of knowing that, having no 
        particular expertise in snakes. She seems content to stay where she is but you would rather keep your distance. ",
        before [;
            talk:
            "She stares at you silently ";

            pet:
            "Nope. Not gonna happen. ";

            take:
            "When you were six you were terrified by the goofy snake in 'Jungle Book' and it scarred you for life. So, no. ";
        ],
        life [;
            show, give:
            "She'd much prefer a gerbil or mouse. ";
        ],
    has animate female proper concealed;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor northrup_door "fancy mahogany door" 
    with name 'fancy' 'mahogany' 'door' 'south',
        description [;
            print"It's a thick dark wooden door, currently ";
            open_or_closed(self);
            if (real_location == admin_hallway) ". A small brass placard on it reads: ~Walt Northrup, M.D., President/CEO~";
                ". You can lock or unlock it from this side. ";
        ],
        door_to [;
            if (parent(self) == northrup_office) return admin_hallway; return northrup_office;
        ],
        door_dir [;
            if (parent(self) == northrup_office) return n_to; return s_to;
        ],
        before [;
            knock:
            if (northrup in northrup_office) "There's no reply, even though you could swear you hear breathing 
            coming from the other side. "; "No one seems to be home. ";

            myunlock:
            if (self hasnt locked) "It's already unlocked. ";
            if (real_location == northrup_office)
            {
                give self ~locked;
                "With a ~click~ you unlock the door. ";
            }
            "You don't seem to have a way to unlock it from this side. ";

            mylock:
            if (self has locked) "It's already locked. ";
            if (real_location == northrup_office)
            {
                if (self has open)
                {
                give self ~open;
                print "(first closing the door)^";
                }
                give self locked;
                "With a ~click~ you lock the door. ";
            }
            "You don't seem to have a way to lock it from this side. ";
        ],
        found_in admin_hallway northrup_office,
    has scenery door openable ~open locked lockable;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room northrup_office "Northrup's Office"
    with description[;
        print"This is a large office belonging to Dr. Walter Northrup, the president and CEO of the hospital. 
        An imposing mahogany desk dominates the room and large wooden bookshelves line the walls. The walls are adorned 
        with the requisite paintings and the floor is covered in thick beige carpet.
        A black safe squats in one corner and a file cabinet in another. The room smells vaguely of pipe smoke";
        if (northrup_chair in self) " and a plush leather executive chair is here next to the desk. "; else ".";
        ],
        cheap_scenery
        3 'thick' 'beige' 'carpet' "It's a standard low pile beige carpet. ",
        before [;
            smell:
                if (noun == 0) "It smells like an attic full of old books and expensive pipe tobacco. ";
            go:
            if (selected_direction == u_to)
            {
                "Even with the file cabinet, the opening in the ceiling is out of reach. ";
            }
            examine:
            if(selected_direction == u_to) "You see a tile drop ceiling. ";
            if(selected_direction == d_to) "You see beige carpet. ";
        ],
    n_to northrup_door,
    u_to ceiling_05,
    has light;

Object bookcase "bookcase" northrup_office
    with 
        parse_name [ w1;
            w1 = NextWord();
            if (w1 == 'shelf' or 'shelves' or 'bookcase' or 'bookshelf' or 'bookshelves') return 1;
        ],

        name 'bookcase' 'shelves' 'bookshelf' 'shelf',
        description "They're dark wooden shelves, seemingly hand-made to match the desk. They're full of 
            numerous books which you don't need. ",
    has scenery;

Object medical_books "medical books" northrup_office
    with 
        name 'medical' 'book' 'books' 'journals',
        description "You see numerous books and medical journals. Nothing that appeals to you. ",
        before [;
            take:
            "While you could stand to be a bit more well-read, you don't need these particular books. ";
            read:
            "You already know everything you need to know about the pineal gland. ";
        ],
    has scenery pluralname;

Object northrup_paintings "paintings" northrup_office
    with
        name 'painting' 'paintings',
        description "They're boring pictures of landscapes and seascapes. ",
        before [;
            take:
            "They're lovely but you don't need them. ";
        ],
    has scenery pluralname;

Object northrup_desk "mahogany desk" northrup_office
    with
        name 'mahogany' 'desk',
        description "It's carved from mahogany and looks very old and very expensive. ",
        before [;
            take:
            "You're just begging for a hernia. ";
        ],
    has scenery supporter;

Object file_cabinet "file cabinet" northrup_office
    with 
        parse_name [ w1 ;
            w1 = NextWord();
            if (w1 == 'metal' or 'gray' or 'file' or 'cabinet' or 'drawer' or 'drawers') return 1;
        ],
        description"It's a standard gray metal file cabinet with three large drawers. ",
        react_before [;
            go:
            if(selected_direction == u_to && player in self)
            {
                "Even syanding on the file cabinet you can't reach the opening in the ceiling. ";
            }
            if(player in self) 
            {
                PlayerTo(northrup_office, 1);
                print"(first getting down from the file cabinet)^";
                <look>;
                if (selected_direction ~= d_to) "^You can't go that way. ";
                rtrue;
            }
        ],
        before [;
            enter, climb:
            PlayerTo(self, 1);
            "With some difficulty, you awkwardly clamber onto the file cabinet. ";
            exit, getoff:
            PlayerTo(northrup_office, 1);
            "You lower yourself from the file cabinet. ";
            open:
            "The drawers are all securely locked. ";
            take:
            "It's far too heavy. ";
        ],
    has supporter scenery enterable;

InChair northrup_chair "leather chair" northrup_office
    with name 'plush' 'leather' 'executive' 'chair',
        description"It's a large plush executive leather chair. ",
        before [;
            if (player in file_cabinet) "You can't reach that from up here. ";
            take:
                "It's too heavy. ";
        ],
        describe [;
            if (self in northrup_office) rtrue;
        ],
    has scenery;

Object northrup_safe "safe" northrup_office 
    with name 'safe',
        description [ ;
            print"It's a squat black metal safe, about two feet on each side. It's currently ";
            open_or_closed(self);
            ". A dial is embedded in the safe door. The numbers 1 through 35 circle it. The dial 
            is currently set to ",SAFE_CURRENT,".";
        ],
        before [;
            if (player in file_cabinet) "You can't reach that from up here. ";
            take:
                "It's far too heavy. ";

            myunlock:
            if (self hasnt locked) "It's already unlocked. ";
            "The only way to unlock it is with the combination. ";

            mylock:
            if (self has locked) "It's already locked. ";
            if (self has open) print"You close the safe. ";
            give self ~open;
            give self locked;
            "You spin the dial. ";
        ],   
        max_capacity 10,
    class MyContainer
    has locked openable scenery ~open;

Object combo_dial "dial" northrup_office
    with
        name 'dial' 'combination',
        description [;
            print"It's the dial to a combination lock. The numbers 1 through 35 circle the outer
            diameter and it's currently set to ",SAFE_CURRENT,".^";
        ],
        before [;
            if (player in file_cabinet) "You can't reach that from up here. ";
            TurnTo:
            if (second < 1 || second > 35) "The numbers only go from 1 to 35.";
            print"You turn the dial to ",second,".^";
            SAFE_CURRENT = second;
            if (SAFE_CYCLE == 0 && second == SAFE_1) 
            {
                SAFE_CYCLE = 1;
                rtrue;
            }
            if (SAFE_CYCLE == 1 && second == SAFE_2)
            {
                SAFE_CYCLE = 2;
                rtrue;
            } 
            if (SAFE_CYCLE == 2 && second == SAFE_3)
            {
                give northrup_safe ~locked;
                Achieved(14);
                SetFlag(F_SAFE_IS_OPEN);
                "You hear a faint ~click~ from within the safe door. "; 
            }  
            SAFE_CYCLE = 0;
            rtrue;                     
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_2 "Second Floor Elevator Lobby" 
    with description [;
        print"This is the second-floor elevator lobby. The elevator doors lie to the south, a small panel next to 
        them. The elevator doors are currently ";
        open_or_closed(elevator_doors); 
        ". A stairwell lies to the north through an open doorway. You see a red sign here that reads ~Ward A~ above an 
        arrow pointing down the hallway to the west and a blue sign reading ~Ward B~ above an arrow pointing east. 
        To assist the confused or dense, a red line starts here on the floor and leads west and a similar blue 
        line leads east. ";
    ],
    cheap_scenery
    'sign' 'arrow' "The sign reads ~Ward B~ and the arrow points east. ",
    n_to stairwell_2,
    e_to hallway_2_3,
    !e_to radiology,
    s_to elevator_doors,
    in_to elevator_doors,
    w_to hallway_2_1,
    class Tiles DropCeiling
    has light;

Object red_line "red line" 
    with 
        name 'red' 'line',
        description "It's a thick red line painted on the floor down the middle of the hallway. ",
        found_in elevator_lobby_2 hallway_2_1 hallway_2_2 a_ward_1,
    has scenery;

Object blue_line "blue line" 
    with 
        name 'blue' 'line',
        description "It's a thick blue line painted on the floor down the middle of the hallway. ",
        found_in elevator_lobby_2 hallway_2_3 b_ward_1 station_b,
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_2_3 "Second Floor Hallway East" 
    with description "This long dim hallway continues east towards ward B and west 
        toward the elevator. A thick blue line is painted down the middle of the floor. ",
    w_to elevator_lobby_2,
    e_to b_ward_1,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room x_ray "X-Ray Suite" 
    with 
        description [;
        print"This white-tiled room is occupied by a metal exam table positioned below the x-ray camera 
        hanging down from rails in the ceiling. White translucent viewing boxes are mounted to one wall; these
        are currently turned ";
        if (viewing_boxes has on) { print "on"; } else { print "off"; }
        ". The exit lies through an open arch to the south and there's an unmarked door to the east. ";
        ],
        s_to basement_hallway_east,
        e_to darkroom_door,
    class Tiles DropCeiling
    has light;

Bed x_ray_table "x-ray table" x_ray 
    with name 'metal' 'exam' 'table',
        description"It's a very uncomfortable looking metal exam table. ",
        before [;
            take:
                "It's securely bolted to the floor. ";
        ];

Object x_ray_camera "camera" x_ray
    with name 'x-ray' 'xray' 'xr' 'camera',
        description"It's a bulky metal and glass x-ray camera, fixed to rails in the ceiling. It can be pushed or pulled for easier 
        positioning above the patient. ",
        before [;
            take:
                "It's attached to the rails in the ceiling. ";
            push, pull:
                "You push and pull the camera back and forth along the rails. Wheee! ";
        ],
    has scenery;

Object rails "rails" x_ray 
    with name 'rail' 'rails//p',
        description"They're metal rails embedded in the ceiling. They allow for easy positioning of the x-ray camera. ",
        before [;
            take:
                "You can't reach them and don't need them. ";
        ],
    has scenery pluralname;

Object viewing_boxes "viewing boxes" x_ray 
    with name 'viewing' 'box' 'boxes//p',
        description [;
                print"It's a row of light boxes used to view x-rays. They're currently ";
                if (self has on) {
                    print"on and flickering with a dull white light. ";
                }   else {
                    print"turned off. ";
                }
                "There's a switch on the side. ";
        ],
        before [;
            switchon:
                if (self hasnt on) { give self on; "You flick the switch and the viewing boxes flicker alight."; } "They're already on. ";
            switchoff:
                if (self has on) { give self ~on; "You flick the switch and the viewing boxes flicker off. "; } "They're already off. ";
        ],
    has scenery switchable pluralname;

Object box_switch "switch" x_ray
    with name 'switch' 'toggle',
        description"It's a metal toggle switch. ",
        before [;
            push, pull, flick:
                if (viewing_boxes has on)   <<switchoff viewing_boxes>>; else <<switchon viewing_boxes>>;
            take:
                "It's fixed to the viewing boxes. ";
            switchon:
                <<switchon viewing_boxes>>;
            switchoff:
                <<switchoff viewing_boxes>>;
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor darkroom_door "door" 
    with 
        name 'unmarked' 'door',
        description [;
            print"It's a plain door, currently ";
            open_or_closed(self);
        ],
        door_to [;
            if (parent(self) == x_ray) return darkroom; return x_ray;
        ],
        door_dir [;
            if (parent(self) == x_ray) return e_to; return w_to;
        ],
        found_in x_ray darkroom,
    has scenery door openable open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room darkroom "Darkroom" 
    with 
        description "This is a small darkroom off of the main x-ray suite. There's a metal basin and tiled counter running 
            along one wall and metal shelves stacked neatly with bottles of chemicals along another. There's a strong 
            chemical smell in here and if you stay much longer you may get a headache. The exit lies through a door
            to the west. ",
        w_to darkroom_door, 
        before [;
            smell:
            "It smells like when your grandmother cleans her oven. ";
        ],
        cheap_scenery
        4 'bottles' 'chemicals' 'bottle' 'chemical' "They look toxic and best left to the experts. ",
    class Tiles DropCeiling
    has light;

MyContainer darkroom_basin "basin" darkroom 
    with 
        name 'metal' 'basin',
        description "It's a metal basin built into the counter. ",
        max_capacity 20,
    has scenery container open; 

Object darkroom_counter "counter" darkroom
    with 
        name 'tiled' 'counter',
        description "It's a tile counter, stained by years of chemical spills. ",
    has scenery supporter;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_2_1 "Second Floor Hallway @@64 Service Elevator"
    with description "Here the hallway continues east and west. A service elevator is here to the north. A sign next to it 
        reads ~Employees Only~. A red line is painted down the middle of the floor. ",
        e_to elevator_lobby_2,
        w_to hallway_2_2,
        n_to service_elevator_door,
        in_to service_elevator_door,
    class Tiles DropCeiling
    has light;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_2_2 "Second Floor Hallway @@64 Dumbwaiter" 
    with description "This is the entrance to 'A' Ward which lies down the hallway to the west. The wallpaper here is red, 
        matching the red line running down the middle of the floor. A sign next to an open doorway to the north reads ~Employees Only~.
        The hallway returns to the east and an old service dumbwaiter is here, embedded in the south wall. ",
        e_to hallway_2_1,
        w_to a_ward_1,
        n_to break_room,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room a_ward_1 "Ward A - East"
    with description "The hallway continues east and west. Here the walls are red to match the line running 
        down the middle of the floor. Patient rooms lie north and south through open doorways. A sign next to the former 
        reads ~23-A~, one next to the latter reads ~24-A~.",
        cheap_scenery
        'south' 'sign' "It reads ~24-A~"
        'north' 'sign' "It reads ~23-A~", 
        e_to hallway_2_2,
        w_to station_a,
        n_to room_23,
        s_to room_24,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_23 "Room 23"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to a_ward_1,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ]
    class Tiles DropCeiling
    has light;

Bed room_23_bed "hospital bed" room_23
    with name 'hospital' 'bed',
        description "It's a standard hospital bed. ",
        before [;
            enter:
                "It's not a sleepover. ";
        ];  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_24 "Room 24"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads north back out to the hallway. ",
        n_to a_ward_1,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ],
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room station_a "Ward A - West"
    with description "This is the western end of a long east-west corridor. The walls are red and open doorways to the north and south 
        lead to patient rooms, signs next to the former read ~21-A~, the latter ~22-A~. The nurses' station is here. 
        It's a large square waist-high desk used for charting and note-taking. ",
        cheap_scenery
        'south' 'sign' "It reads ~22-A~"
        'north' 'sign' "It reads ~21-A~",
        e_to a_ward_1,
        n_to room_21,
        s_to room_22,
    class Tiles DropCeiling
    has light;

Object ward_a_station "nurses' station" station_a
    with name 'nurse' 'station' 'desk',
        description "It's a tall standing desk, square and roughly waist high. It gives the nurses and physicians 
        a surface to update notes and charting. ",
    has supporter scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_21 "Room 21"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to station_a,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ],
    class Tiles DropCeiling
    has light;

Bed room_21_bed "hospital bed" room_21
    with name 'hospital' 'bed',
        description "It's a standard hospital bed. ",
        before [;
            enter:
                "It's not a sleepover. ";
        ];  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_22 "Room 22"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads north back out to the hallway. ",
        n_to station_a,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            print(string)RADIATOR_DESC; rtrue;
            take, touch, pull, push:
            print(string)RADIATOR_TOUCH; rtrue;
        ],
    class Tiles DropCeiling
    has light;

Bed room_22_bed "hospital bed" room_22
    with name 'hospital' 'bed',
        description "It's a standard hospital bed. ",
        before [;
            enter:
                "You might accidentally crush Ms. Parsons. ";
        ];  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room break_room "Break Room"
    with 
        description "This is an employee break room. There's a white plastic table here with matching chairs. A 
        refrigerator stands in the corner next to a stained formica counter. The doorway out is to the south. ",
        s_to hallway_2_2,
        cheap_scenery
        'formica' 'counter' "It's a standard formica counter. It's adorned with years of stains and scorch marks. "
        4 'white' 'plastic' 'chair' 'chairs' [;
            examine:
                "You see a number of cheaply-made white plastic chairs. ";
            take:
                if (break_room_chair in self) rfalse; "You've already taken one of the chairs. ";
        ]
        1 'table' "It's a white, dirty, poorly made plastic table. ",
    class Tiles DropCeiling
    has light;

OnChair break_room_chair "vinyl chair" break_room
    with name 'white' 'vinyl' 'chair',
        description "It's a rather flimsy white vinyl chair. ",
        describe [; if(self in break_room) rtrue; ],
        before [;
            take:
                if(self hasnt moved) print"You pick one of the ugly white chairs.^";
            enter:
                if(self hasnt moved) print"You pick one of the ugly white chairs.^";
        ],            
        mass 15;

Object refrigerator "refrigerator" break_room 
    with name 'refrigerator' 'fridge',
        description "It's an old off-white refrigerator. A handwritten note is taped to the front. ",
        before [;
            take:
                "You don't possess the power of levitation. You do seem to possess the power of delusion. ";
            receive:
            "It's already packed full of leftovers and lunch bags. ";
        ],
    has scenery container openable ~open;

Object fridge_sign "refrigerator sign" break_room
    with name 'sign' 'handwritten' 'note',
        description "It's a handwritten note outlining standard breakroom etiquette. ",
        before [;
            read:
                "~Your mother doesn't work here so CLEAN UP AFTER YOURSELF! Label your food and ONLY EAT WHAT YOU BRING!!~ The 
                abundance of exclamation marks and upper-case letters nearly give you a headache. ";
            take:
                "Then how would people know that their mother doesn't work here?";
        ],
    has scenery;

Object fridge_stuff "smelly collection of Tupperware and condiments" refrigerator
    with name 'smelly' 'collection' 'Tupperware' 'condiments',
        description "It's an assortment of plastic containers each one labelled with the owner's name. You can also see various bottles and 
            containers of out-of-date condiments. ",
        before [;
            take:
                    "None of it is yours. Didn't you read the sign?";
            smell:
                    "You smell a complex melange of old food and baking soda. ";
        ],
    has static;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sub_basement_02 "Sub-Basement @@64 Service Elevator"
    with description "This is the middle of a short dimly lit corridor that continues to the east and west.  
        The floor and walls here are dark gray concrete and fluorescent tubes flicker and buzz annoyingly from above. 
        A service elevator is here to the north. A sign next to it reads ~Employees Only~.",
    n_to service_elevator_door,
    in_to service_elevator_door,
    e_to sub_basement_03,
    w_to sub_basement_01,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sub_basement_03 "Sub-Basement East"
    with description [;
        print"This is the eastern end of a short dimly lit corridor that returns to the west.  
        The floor and walls here are dark gray concrete and fluorescent tubes flicker and buzz annoyingly from above. 
        A plain door here to the south is labelled ~Restroom~. ";
        if (vic in bathroom) print(string) BATHROOM_OCCUPIED; else print(string) BATHROOM_VACANT;
        "";            
    ],
    cheap_scenery
    3 'red' 'occupied' 'sign' "It's a small red ~Occupied~ sign in the door. "
    3 'green' 'occupied' 'sign' "It's a small green ~Vacant~ sign in the door. "
    'fluorescent' 'tubes' "They're standard industrial fluorescent lights. "
    'wall' 'floors' "You see unremarkable industrial concrete. ",
    w_to sub_basement_02,
    s_to bathroom_door,
    in_to bathroom_door,
    has light;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor bathroom_door "bathroom door"
     with
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'bathroom' && w2 == "door") return 2;
            if (w1 == 'restroom' && w2 == "door") return 2;
            if (w1 == 'door') return 1;
        ],
        description [;
            print"It's an unassuming bathroom door, currently ";open_or_closed(self);print". ";
            if(real_location == sub_basement_03 && vic in bathroom) print(string) BATHROOM_OCCUPIED;
            if(real_location == sub_basement_03 && vic notin bathroom) print(string) BATHROOM_VACANT;
            "";
        ],
        door_to [;
            if (parent(self) == sub_basement_03) return bathroom; return sub_basement_03;
        ],
        door_dir [;
            if (parent(self) == sub_basement_03) return s_to; return n_to;
        ],
        before [;
            knock:
            "There's no answer but you could swear you hear breathing from the other side of the door. ";

            mylock:
            if (self has locked) "It's already locked. ";
        
            if (real_location == bathroom)
            {
                if (self has open)
                {
                give self ~open;
                print "(first closing the door)^";
                }
                give self locked;
                "You lock the door with a ~click~. ";
            }
            "You can't lock it from this side. ";

            myunlock:
            if (self hasnt locked) "It's already unlocked. ";
            if (real_location == bathroom)
            {
                give self ~locked;
                "You unlock the door with a ~click~. ";
            }
            "You can't unlock it from this side. ";
        ],
        found_in sub_basement_03 bathroom,
    has scenery door openable ~open locked;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object bathroom "Bathroom"
    with
        description "It's a small minimal bathroom with a toilet, faucets, and sink. It smells really bad in here and 
        you long for the relative fresh air of the hallway. ",
        cheap_scenery
        'sink' 'basin' "It's a chipped porcelain sink that was once white but has yellowed with time. "
        4 'faucets' 'faucets' 'tap' 'taps' "They're standard metal handles, fairly rusted. "
        1 'toilet' "Well, it's a toilet. There's nothing in it, thankfully. ",
        n_to bathroom_door,
        out_to bathroom_door,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sub_basement_01 "Sub-Basement West" 
    with description "This is the western end of a depressingly dim gray hallway. It continues to the east and a
        battered metal door lies to the west. There's a red sign on the door, nearly rusted past 
        legibility. It reads ~Caution: Steam~. An old dumbwaiter is embedded in the wall to the south. ",
        after [;
            go:
            !if (selected_direction == e_to && vic notin sub_basement_02) move vic to sub_basement_02;
        ],
    e_to sub_basement_02,
    w_to boiler_door,
    class Tiles DropCeiling
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor boiler_door "boiler room door"
     with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'boiler' && w2 == 'room' && w3 == 'door') return 3;
            if (w1 == 'boiler' && w2 == 'door') return 2;
            if (w1 == 'door') return 1;
        ],
        description [;
            print"It's a heavy thick metal door, currently ";open_or_closed(self);".";
        ],
        door_to [;
            if (parent(self) == sub_basement_01) return boiler_room; return sub_basement_01;
        ],
        door_dir [;
            if (parent(self) == sub_basement_01) return w_to; return e_to;
        ],
        found_in sub_basement_01 boiler_room,
    has scenery door openable ~open; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object boiler_room "Boiler Room" 
    with description "You're standing in a high arched stone and concrete-lined space populated by massive black 
    boilers, squatting against the walls like sleeping dinosaurs. They're all closed and covered with thick coats of dust. 
    A half-dozen metal lamps hang down from the ceiling above, although you can't actually make out the ceiling in the gloom.
    A dented thick metal door leads east. It looks like someone has been killing time here and it smells like your Uncle Vincent. ",
        cheap_scenery
        'boiler' 'boilers' "Each boiler is a black metal hulk squatting on the concrete and stone floor. "
        5 'metal' 'lamp' 'lamps' 'light' 'lights' "They're dusty black lamps hanging from the darkness above. ",
        after [;
            go:
            if (selected_direction == w_to && vic notin sub_basement_02)
            {
                move vic to sub_basement_02;
                give bathroom_door ~locked;
                give bathroom_door open;
                print"^You hear a door opening to the east down the hall.^^";
            }
        ],
        before [;
            smell:
            "It smells like aftershave and cigarettes. ";
            examine:
            if(selected_direction == u_to)  "You see a tangle of pipes and ducts. ";
            if(selected_direction == d_to)  "The floor is gray concrete. ";
        ],
    e_to boiler_door,
    has light;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_01 "In The Ceiling"
    with description [;
        print"You are perched in the dark service space within the ceiling. Your flashlight reflects 
        red light off of dust motes as you support yourself on thick pipes and brackets suspended from higher up. Large 
        air ducts block any movement to the east but the way to the west is open. Here 
        there is a gap in the drop-ceiling tiles below you and you can see a ladder down below.^";
        ],
        before [;
            go:
            if (selected_direction == d_to && ladder in hallway_m2) 
                if (ladder hasnt open) { "The ladder is folded up. "; }
                else
                {
                print"You gingerly lower yourself through the gap in the drop ceiling, holding onto 
                piping in the ceiling for support until you feel your feet land on the top of the aluminum 
                ladder. Balancing carefully, you finally climb down, relieved to be back on solid ground.^^";
                PlayerTo(hallway_m2);
                rtrue;
                }
            examine:
            if (selected_direction == d_to) "You notice one of the large tiles in the drop-ceiling here out of alignment. 
                Through the gap you can see the hallway and a ladder below. ";
        ],
        cheap_scenery
            CS_ADD_LIST ceiling_01 (inside_scenery),
        inside_scenery
            'pipe' 'pipes' [;
            examine:
            "It's a network of black pipes running through the ceiling. ";
            take:
            "Actually, no. ";
            ]
            'bracket' 'brackets' [;
            examine:
            "It's a boxy structure of metal brackets holding up the drop ceiling below. ";
            take:
            "They're currently holding you up. ";
            ]
        3 'duct' 'ducts' 'air' [;
            examine:
            "They're boxy aluminum ducts coated with dust. ";
            take:
            "You definitely don't need that. ";
        ],    
        w_to ceiling_03;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_03 "In The Ceiling"
    with 
        description "You are perched in the service space above the hallway and drop ceiling, surrounded by 
        dusty ducts and pipes. The dusty passage takes a ninety-degree turn here and you can go east or south, balancing 
        yourself on the metal brackets holding up the drop ceiling tiles below. ",
        cheap_scenery
            CS_ADD_LIST ceiling_01 (inside_scenery),
        e_to ceiling_01,
        s_to ceiling_05;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_05 "In The Ceiling"
    with description [;
        print"You are perched in the dark service space within the ceiling. The passage dead-ends here, 
        and movement to the south is blocked by a large duct. You can crawl north along a network of 
        pipes and brackets into darkness. A tile in the drop-ceiling below is askew, allowing you to look 
        down into an office below. Directly underneath you lies a tall file cabinet that you could probably
        lower yourself down onto, although you probably won't be able to get back up this way. ";
        if (northrup in northrup_office) print"^Below you, Dr. Walt Northrup is seated at a large mahogany desk. ";
        if (nurse_retch in northrup_office) print"Nurse Retch is down there as well. ";
        "";
        ],
        cheap_scenery
            CS_ADD_LIST ceiling_01 (inside_scenery)
            'ceiling' 'tile' [ ;
                examine:
                "It's an out-of-place tile in the drop ceiling. ";
                take, push, pull:
                print"As you manipulate the ceiling tile, you release tiny spates of dirt and dust that fall 
                downwward into the office below. Suddenly, you find yourself staring down into the angry face
                of Dr. Northrup who is none-to-happy to see a candy striper trying to burgle his office.^^
                Obviously, you do not get credit for your volunteer time at St. Asterix.";
                deadflag = 3;
                rtrue;
            ],
        before [;
            go:
            if (selected_direction == d_to) 
            {
                print"Grunting and dust-covered, you lower yourself from the pipes through the drop ceiling, 
                stepping down first onto a tall metal file cabinet standing in the corner of the room, then down onto 
                the plush carpet.^^";
                if (northrup in northrup_office) 
                {
                    print"A seething Dr. Walter Northrup is here to meet you, furious in equal parts at the cloud of dust you've
                    brought down onto his carpet as well as your ill-conceived invasion of his office. Unsurprisingly, 
                    your time as a candy-striper is at an end.";
                    deadflag = 3;
                    rtrue;
                }
            }
            examine:
            if (selected_direction == d_to) "You notice one of the large tiles in the drop-ceiling here out of alignment. 
                Through the gap you can see an office and a file cabinet below. ";
        ],

        each_turn [;
            if (northrup in northrup_office && nurse_retch in northrup_office)
            {
                remove northrup;
                remove nurse_retch;
                "^There's a heated conversation going on down below. You can't make it out in its entirety, only fragments. 
                ~We have to find that ledger before the cops get here,~ you hear Northrup say.~
                ~I don't think the candy striper actually knows anything. If she gets too nosy my brother can take care of her,~ 
                you hear from Nurse Retch.^^
                After a moment of agitated back-and-forth, Northrup stands up and they leave the office together. You hear the door close behind
                them and then the ~click~ of the lock. ";
            }
        ],
        n_to ceiling_03,
        d_to northrup_office;



