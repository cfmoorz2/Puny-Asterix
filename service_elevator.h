!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Class ServiceButton 
    with call_level,
        description [;
            print(The)self, " is currently ";
            if (self has on) "lit"; else "unlit";
        ],
        before [;
            push:
                if (FlagIsClear(F_SERVICE_ELEVATOR_ON)) "The service elevator doesn't seem to be working. ";
                if (self has on) "Pressing it again won't speed things up. "; 
                if (service_elevator_active) "Nothing seems to happen. ";
                switch(real_location)   {
                    sub_basement_02: self.call_level = -1;
                    basement_hallway_west: self.call_level = 0;
                    !hallway_m1: self.call_level = 1;
                    hallway_2_1: self.call_level = 2;
                    !hallway_3_1: self.call_level = 3;
                    !print "^here elevator call level = ",elevator_call_level," self.call.level = ",self.elevator_call_level,"";
                }
                if (service_elevator_level == self.call_level)   {
                    give service_elevator_door open;
                    StartTimer(service_close_door_timer, 3);
                    ResetServiceButtons();
                    service_elevator_active = false;
                    "^With a 'ding' the elevator door opens. ";
                }   else    {
                    give self on;
                    service_elevator_call_level = self.call_level;
                    StartDaemon(service_elevator_daemon);
                    service_elevator_active = true;
                    StopTimer(service_close_door_timer);
                    if (service_elevator_door has open) print"The elevator door slides closed. ";
                    "The button lights and you hear mechanical humming from the elevator shaft. ";
                    }
        ],
    has scenery;

[ResetServiceButtons obj;
    objectloop(obj ofclass ServiceButton)  {
        give obj ~on;
    }
];

[ShowServiceButtons obj;
    objectloop(obj ofclass ServiceButton)  {
        if (obj has on) print" ",(The)obj," is currently lit.^";
    }
];

Object service_elevator "Service Elevator"
    with description [;
        print"This is the service elevator. The floor is tile, the walls dented metal. There's a panel of buttons next to the single 
        door which is currently ";
        open_or_closed(service_elevator_door); ".";
        ],
        s_to service_elevator_door,
        out_to service_elevator_door,
        before [;
            go:
            if (selected_direction == out_to && FlagIsSet(F_HAVE_MET_VIC) && vic in sub_basement_02) { remove vic; StopDaemon(vic); }
        ],
    has light;

Object service_elevator_door "service elevator door"
    with name 'door',
        description [;
            print"It's a single metal sliding door. It's currently ";
            open_or_closed(self);". ";
        ],
        door_dir [;
            if (self in service_elevator) return s_to; else return n_to;
        ],
        door_to [;
            if (self in service_elevator) {
                switch(service_elevator_level)  {
                    -1: return sub_basement_02;
                    0:  return basement_hallway_west;
                    !1:  return hallway_m1;
                    2:  return hallway_2_1;
                    !3:  return hallway_3_1;
                    }
                } else {
                    return service_elevator;
            }
        ],
    !found_in service_elevator basement_hallway_west hallway_m1 hallway_2_1 hallway_3_1 sub_basement_02,
    found_in service_elevator basement_hallway_west hallway_2_1 sub_basement_02,
    has scenery door ~open; 

Object service_elevator_ext "service elevator"
    with name 'service' 'elevator',
        description [;
            print"It's a utilitarian service elevator with a single metal door, currently ";
            open_or_closed(service_elevator_door);
            " There's a small panel embedded in the wall next to it. ";
        ],
        !found_in basement_hallway_west hallway_m1 hallway_2_1 hallway_3_1 sub_basement_02,
        found_in basement_hallway_west hallway_2_1 sub_basement_02,
    has scenery;

Object service_interior_panel "panel" service_elevator
    with name 'panel' 'buttons',
        description [; 
            print"It's a panel next to the door. You see five buttons. One is labeled with two horizontal arrows pointing away from each other. 
                The others are labeled: ~B~,  ~M~, and ~2~. ";
            if (service_elevator_active == false) print"None of the buttons are lit. ";
            print"Below the buttons there's a magnetic swipe card reader. Next to this it reads 'Sub-Basement'.^";
            ShowServiceButtons();
        ],
    has container transparent scenery;

Object card_reader "card reader" service_elevator
    with
        parse_name [w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'card' && w2 == 'reader') return 2;
            if (w1 == 'reader' && w2 == 0) return 1;
            if (w1 == 'slot' && w2 == 0) return 1;
        ],
        description [;
            print "It's a magnetic swipe card reader built into the interior panel of the elevator. There's a little light 
            on it which is ";
            if (self has on) "green. "; else "red. ";
        ],
    has scenery;

Object service_interior_open_button "open button"
    with name 'open' 'button',
        description "It's an unlit button labelled with two horizontal arrows pointing away from each other. ",
        before [;
            push:
                if (service_elevator_door has open)    {"The light briefly flickers but nothing happens otherwise. ";
                }   else {
                    give service_elevator_door open;
                    StartTimer(service_close_door_timer, 3);
                    service_elevator_active = false;
                    "^With a cheery 'ding' the elevator door slides open. ";
                }
        ],
        found_in service_interior_panel,
        has scenery;   

ServiceButton  service_interior_m_button 
    with name 'm//' 'main' 'button',
        call_level 1,
        short_name "main button",
        found_in service_interior_panel;     

ServiceButton  service_interior_2_button 
    with name '2//' 'two' 'second' 'button',
        call_level 2,
        short_name "second floor button",
        found_in service_interior_panel;  

ServiceButton  service_interior_b_button 
    with name 'b//' 'basement' 'button',
        call_level 0,
        short_name "basement button",
        found_in service_interior_panel;   

ServiceButton  service_interior_sb_button 
    with name 'sb' 'sub-basement' 'button' 'subbasement',
        call_level -1,
        short_name "sub-basement button",
        before [;
            push:
            "It flashes on then off. Nothing otherwise seems to happen. ";
        ],
        found_in service_interior_panel;     

Object sub_basement_exterior_panel "panel" sub_basement_02
    with name 'panel' 'buttons',
        description"It's a small panel embedded in the wall next to the elevator door. It contains a single button. ",
    has scenery;

Object service_exterior_panel "panel"
    with name 'panel',
        description [;
            print"It's a panel embedded in the wall next to the elevator door. It contains 'Up' and 
            'Down' buttons.^";
            if (service_elevator_active == false)    "None of the buttons are lit. ";
            ShowServiceButtons();
        ],
    found_in basement_hallway_west hallway_m1 hallway_2_1,
    has container transparent scenery;

ServiceButton  service_exterior_up_button 
    with name 'up' 'button',
        short_name "up button",
        found_in service_exterior_panel;

ServiceButton  service_exterior_down_button
    with name 'down' 'button',
        short_name "down button",
        found_in service_exterior_panel;

ServiceButton  sb_service_exterior_up_button
    with name 'up' 'button',
        short_name "up button",
        found_in sub_basement_exterior_panel;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object service_elevator_daemon 
    with daemon [ ;
        if (service_elevator_call_level == service_elevator_level) {
            give service_elevator_door open;
            StopDaemon(self);
            StartTimer(service_close_door_timer,3);
            ResetServiceButtons();
            service_elevator_active = false;
            "^With a 'ding' the elevator door opens. ";
            }
        if (service_elevator_call_level < service_elevator_level) 
        {
            if (service_elevator_level == 2) { service_elevator_level = 0; rtrue; }
            service_elevator_level--;
        }
        if (service_elevator_call_level > service_elevator_level)
        {
            !print"^elevator level =",service_elevator_level,"^";
            !print"^call level = ",service_elevator_call_level,"^";
            if (service_elevator_level == 0) { service_elevator_level = 2; rtrue; }
           service_elevator_level++;
        }        
        ];

Object service_close_door_timer 
    with time_left,
        time_out[;
            give service_elevator_door ~open;
            ResetServiceButtons();
            if (TestScope(service_elevator_door, player)) "^With a 'ding', the elevator door slides closed. ";
        ];








