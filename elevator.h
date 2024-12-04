!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Class Button 
    with call_level,
        description [;
            print(The)self, " is currently ";
            if (self has on) "lit. "; else "unlit. ";
        ],
        before [;
            push:
                if (self has on) "Pressing it again won't speed things up. "; 
                if (elevator_active) "Nothing seems to happen. ";
                !if (elevator_doors has open) "The elevator is already open and waiting. ";
                switch(real_location)   {
                    elevator_lobby_m: self.call_level = 1;
                    elevator_lobby_2: self.call_level = 2;
                    !elevator_lobby_3: self.call_level = 3;
                    !print "^here elevator call level = ",elevator_call_level," self.call.level = ",self.elevator_call_level,"";
                }
                if (elevator_level == self.call_level)   {
                    give elevator_doors open;
                    StartTimer(close_door_timer,3);
                    ResetButtons();
                    elevator_active = false;
                    "^With a 'ding' the elevator doors open. ";
                }   else    {
                    give self on;
                    elevator_call_level = self.call_level;
                    StartDaemon(main_elevator_daemon);
                    elevator_active = true;
                    StopTimer(close_door_timer);
                    if (elevator_doors has open)  { print(string) MAIN_ELEVATOR_PROSE; rtrue; } 
                    else
                    {
                        "The button lights and you hear mechanical humming from the elevator shaft. ";
                    }
                }
        ],
    has scenery;

[ResetButtons obj;
    objectloop(obj ofclass Button)  {
        give obj ~on;
    }
];

[ShowButtons obj;
    objectloop(obj ofclass Button)  {
        if (obj has on) print" ",(The)obj," is currently lit.^";
    }
];

Object main_elevator "Main Elevator"
    with description [;
        print"This is the cramped and dingy main elevator. The floor is worn carpet. The walls are linoleum and 
        fluorescent lights filter down from above through plastic panels. There's a panel of buttons next to the elevator doors
        which are currently ";
        open_or_closed(elevator_doors); ".";
        ],
        n_to elevator_doors,
        out_to elevator_doors,
    has light;

Object main_elevator_ext "elevator" 
    with name 'elevator',
        description"It's a standard elevator with a pair of sliding doors. There's a small panel embedded in the wall next to it. ",
        before [;
            dialobj: "You should use the buttons to summon the elevator. ";
        ],
    found_in elevator_lobby_b elevator_lobby_m elevator_lobby_2,
    has scenery;

Object elevator_doors "elevator doors"
    with name 'door' 'doors',
        description [;
            print"They're a standard pair of elevator doors. They're currently ";
            open_or_closed(self);".";
        ],
        door_dir [;
            if (self in main_elevator) return n_to; else return s_to;
        ],
        door_to [;
            if (self in main_elevator) {
                switch(elevator_level)  {
                    0:  return elevator_lobby_b;
                    1:  return elevator_lobby_m;
                    2:  return elevator_lobby_2;
                    }
                } else {
                    return main_elevator;
            }
        ],
    found_in main_elevator elevator_lobby_b elevator_lobby_m elevator_lobby_2,
    has scenery door ~open pluralname; 

Object interior_panel "panel" main_elevator
    with name 'panel' 'buttons',
        description [; 
            print"It's a panel next to the door. You see five buttons. One is labeled with two horizontal arrows pointing away from each other. 
                The others are labeled: ~B~,  ~M~, and ~2~. ";
            if (elevator_active == false)    "None of the buttons are lit. ";
            ShowButtons();
        ],
    has container transparent scenery;

Object interior_open_button "open button"
    with name 'open' 'button',
        description "It's an unlit button labelled with two horizontal arrows pointing away from each other. ",
        before [;
            push:
                if (elevator_doors has open)    {"The light briefly flickers but nothing happens otherwise. ";
                }   else {
                    give elevator_doors open;
                    StartTimer(close_door_timer,3);
                    elevator_active = false;
                    "^With a cheery 'ding' the elevator doors slide open. ";
                }
        ],
        found_in interior_panel,
        has scenery;   

Button interior_m_button "'M' Button" 
    with name 'm//' 'main' 'button',
        call_level 1,
        !short_name "main button",
        found_in interior_panel;     

Button interior_2_button "'2'Button"
    with name 'two' 'second' '2//' 'button',
        call_level 2,
        parse_name [w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if ((w1 == '2//' or 'two' or 'second') && (w2 == 'floor' or 'button')) return 2;
            if (w1 == '2//' or 'two' or 'second') return 1;
        ],
        !short_name "second floor button",
        found_in interior_panel;  

Button interior_b_button "'B' Button"
    with name 'b//' 'basement' 'button',
        call_level 0,
        !short_name "basement button",
        found_in interior_panel;      

Object basement_exterior_panel "panel" elevator_lobby_b
    with name 'panel' 'buttons',
        description [; print(string)SINGLE_BUTTON_PANEL; ],
    has scenery;

Object second_floor_exterior_panel "panel" elevator_lobby_2
    with name 'panel' 'buttons',
        description [; print(string)SINGLE_BUTTON_PANEL; ],
    has scenery;

Object exterior_panel "panel" elevator_lobby_m
    with name 'panel',
        description [;
            print"It's a panel embedded in the wall next to the elevator doors. It contains 'Up' and 
            'Down' buttons.^";
            if (elevator_active == false)    "None of the buttons are lit. ";
            ShowButtons();
        ],
    has container transparent scenery;

Button exterior_up_button "Up Button" 
    with name 'up' 'button',
        short_name "up button",
        found_in exterior_panel;

Button exterior_down_button "Down Button"
    with name 'down' 'button',
        short_name "down button",
        found_in exterior_panel second_floor_exterior_panel;

Button basement_exterior_up_button "Up Button"
    with name 'up' 'button',
        short_name "up button",
        found_in basement_exterior_panel;



!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object main_elevator_daemon 
    with daemon [ ;
        if (elevator_call_level == elevator_level) {
            give elevator_doors open;
            StopDaemon(self);
            StartTimer(close_door_timer,3);
            ResetButtons();
            elevator_active = false;
            "^With a 'ding' the elevator doors open. ";
            }
        if (elevator_call_level < elevator_level)   { elevator_level--;}
        if (elevator_call_level > elevator_level)   { elevator_level++;}
        !print"^elevator level =",elevator_level;
        !print"^call level = ",elevator_call_level,"^";
        ];

Object close_door_timer 
    with time_left,
        time_out[;
            give elevator_doors ~open;
            ResetButtons();
            if (TestScope(elevator_doors, player)) "^With a 'ding', the elevator doors slide closed. ";
        ];








