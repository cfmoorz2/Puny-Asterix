Attribute touched;
Constant TARGET_PATH = 2;
Constant WANDER_PATH = 1;
Constant FOLLOW_PATH = 3;


Array queue --> 64;
Array position_narrate_array --> 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0;

[ reset_position_array x;
    for (x=0 : x<24: x++) 
    {
    position_narrate_array-->x = 0;
    }
];

[ add_to_position_array obj dir x y;
    for (x=0 : x<24: x++) 
    {
        if (position_narrate_array-->x == 0)
        {
            position_narrate_array-->x = obj;
            y = x + 1;
            position_narrate_array-->y = dir;
            rtrue;
        }
    }
    #IfDef DEBUG;
    "ERROR_POSITION_ERROR_OVERRUN";
    #Endif;
];

Class Room 
    with hop,
         hop_direction,
         each_turn [obj direction one_out npc x y _i _d;
            objectloop (direction in compass)
            {
                one_out = real_location.(direction.door_dir);
                if (one_out ofclass myDoor && one_out has open)
                {
                    one_out = one_out.door_to(direction.door_dir);
                } 
                if (one_out ofclass Room)
                {
                    objectloop (obj in one_out)
                    {
                        if(obj has animate && obj.hide == false)
                        {
                            add_to_position_array(obj, direction);
                        }
                    }
                }
            }
            if (position_narrate_array-->0 ~= 0) print"^";
            for (x=0 : x<24: x++) 
            {
                _i = position_narrate_array-->x;
                if (_i == 0) break;
                y = x + 1;
                _d = position_narrate_array-->y;
                print(The)_i;
                if(_i has pluralname) { print" are ";}  else { print " is "; }
                if(_d == u_obj) print"up above.^";
                if(_d == d_obj) print"down below.^";
                if(_d ~= u_obj && _d ~= d_obj) print"off to the ",(name)_d,".^";
                x++;                
            }
            reset_position_array();
            objectloop (npc && npc ofclass Mover)
            {
                npc.hide = false;
            }
         ],
	has light;

Class myDoor 
    with hop,
        hop_direction,
    has door;

Class Mover 
    with move_mode, !0 = stationary, 1 = wander, 2 = path
        target_room,
        npc_walk [; print"heads";],
        npc_follow [; print"enters";],
        npc_avoid 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0,
        npc_wander_delay,
        npc_last_wander,
        hide false,
        npc_is_following 0,
        npc_arrived [;],
        npc_open_door [ ;
            print (The)self," opens the door. ";
        ],
        npc_post_follow [; rfalse;],
        npc_post_move [; rfalse;],
        daemon [ x y i ;
            if(self.move_mode == TARGET_PATH)
			{
				path_move(parent(self), self.target_room);
			}
            if(self.move_mode == FOLLOW_PATH)
            {
                self.target_room = real_location;
                if (self notin real_location) path_move(parent(self), self.target_room);
                if (self provides npc_arrived) self.npc_arrived(self.target_room); 
            }
            if(self.move_mode == WANDER_PATH)
            {
                x = self.npc_wander_delay;
                y = random(x);
                if (x ~= y) rfalse;
                wander_move();
            }
            objectloop (i in parent(self))  {
                 if(i has valuable) {
                    move i to self;
                    if(self in real_location) print"^",(The)self," picks up ",(the)i,".^";
                }
            }
            rfalse;
        ],
        life [ ;
            if (self notin real_location) print_ret(The)self," isn't here. ";
        ],
        before [;
            give, show:
            if (self notin real_location) print_ret(The)self," isn't here. ";
        ],
        
    has animate;

[enqueue obj i;
    if (obj has touched) rfalse;
    i = 0;
    while (i < 64)
    {
        if (queue-->i == 0) 
        { 
            queue-->i = obj;
            give obj touched;
            rtrue;
        }
            i++;
    }
];

[dequeue temp i k;
    i = 1;
    k = 0;
    temp = queue-->0;
    while ( i < 64) 
    {
        queue-->k = queue-->i;
        i++; k++;
    }
    return temp;
];

[reset_queue i;
    while(i < 64)
    {
        queue-->i = 0;
        i++;
    }
];

[reset_touched obj;
    objectloop (obj ofclass Room || obj ofclass myDoor)
    {
        give obj ~touched;
    }
];

[find_path start_room end_room obj;
    reset_queue();
    reset_touched();
    enqueue(start_room);
    obj = start_room;
    while (obj)
    {
        obj = dequeue();
        if(obj == end_room) rtrue;
        eval_node(obj);
    }
];

[eval_node current_node direction k temp;
    objectloop (direction in compass)
    {
        k = current_node.(direction.door_dir);
        if (k ofclass Room) 
        {
            if(enqueue (k))
            {
                k.hop = current_node;
                k.hop_direction = direction;
            }
        }
        if (k ofclass myDoor)
        {
            move k to current_node;
            temp = k.door_to();
            remove k;
            MoveFloatingObjects();
            if(enqueue(temp))
            {
                temp.hop = current_node;
                temp.hop_direction = direction;
            }
        }
    }
];

[path_move prev final way temp; 
    find_path(parent(self), self.target_room);
    prev = self.target_room;
    while (prev)
    {
        if(prev.hop == parent(self))
        {
            final = prev.hop_direction;
            way = parent(self).(final.door_dir);
            if (way has door)
            {
                if (way in real_location && self provides npc_open_door && way hasnt open) 
                {
                    self.npc_open_door(way);

                }
                move way to parent(self);
                scope_modified = true;
                temp = way;
                way = way.door_to(final.door_dir);
                remove temp;
                MoveFloatingObjects();
            }

            eval_one_away(self, final);
            if (self in real_location && self.move_mode == TARGET_PATH) { self.hide = true; narrate_move(self, final); }
            move self to way;
            scope_modified = true;
            if (self in real_location && self.move_mode == FOLLOW_PATH) { self.hide = true; narrate_move(self, final); }
            if (self provides npc_post_move) self.npc_post_move(); 
            if (self in real_location && self.hide == false)
            {
                print(The)self;
                if(self has pluralname) { print" are ";}  else { print " is "; }
                print"here.^";
            }
            !self.hide = false;
            if (way == self.target_room && self.move_mode == TARGET_PATH) 
            { 
                StopDaemon(self);
                self.move_mode = 0;
                if (self provides npc_arrived) self.npc_arrived(self.target_room); 
                rtrue;
            } else
            break;
        }
		prev = prev.hop;
    }
];

[eval_one_away npc final direction k;
    if(npc.npc_is_following == true)
    {
    objectloop(direction in compass) 
    {
        k = location.(direction.door_dir);
        if (npc in k) 
        {
            if (direction == d_obj) 
            {
                print"^",(The)npc;
                if(npc has pluralname) { print" are ";}  else { print " is "; }
                print "down below heading ";
                if (final == d_obj) "downstairs. ";
                if (final == u_obj) "upstairs. ";
                "to the ",(name)final,".";
            }
            if (direction == u_obj) 
            {
                print"^",(The)npc;
                if(npc has pluralname) { print" are ";}  else { print " is "; }
                print"up above heading ";
                if (final == d_obj) "downstairs. ";
                if (final == u_obj) "upstairs. ";
                "to the ",(name)final,".";
            }
            
            print"^",(The)npc;
            if(npc has pluralname) { print" are ";}  else { print " is "; }
            print"off to the ",(name)direction;
            if (final == d_obj) " heading downstairs. ";
            if (final == u_obj) " heading upstairs. ";
            " heading to the ",(name)final,".";
        }
    }
    }
];

[wander_move npc_at test_room direction i x;
    npc_at = parent(self);
    objectloop (direction in compass)
    {
        test_room = npc_at.(direction.door_dir);
        if (~~test_room ofclass Room) continue;
        if(assess_no_go(self, test_room)) continue;
        i++;
    }
    if (i == 0 ) rfalse;
    if (i == 1) self.npc_last_wander = 0;
    i = 0;
    objectloop (direction in compass)
    {
        if (direction == self.npc_last_wander) continue;
        test_room = npc_at.(direction.door_dir);
        if (~~test_room ofclass Room) continue;
        if(assess_no_go(self, test_room)) continue;
        i++;
    }
    x = random(i);
    i = 0;
    objectloop (direction in compass)
    {
        if (direction == self.npc_last_wander) continue;
        test_room = npc_at.(direction.door_dir);
        if (~~test_room ofclass Room) continue;
        if(assess_no_go(self, test_room)) continue;
        i++;
        if (i == x) 
        { 
            eval_one_away(self, direction);
            if (self in real_location) { self.hide = true; narrate_move(self, direction); }
            move self to test_room;
            scope_modified = true;
            if (self in real_location && self.move_mode == FOLLOW_PATH) { self.hide = true; narrate_move(self, direction); }
            self.npc_last_wander = reverse_dir(direction);
            if (self in real_location) self.describe();
           rtrue;
        }
    }
];

[assess_no_go npc test_room i x;
    for ( i=0: i < 12: i++)
    {
        x = npc.npc_avoid-->i;
        if (x == 0) continue;
        if (x == test_room) rtrue; 
    }
    rfalse;        
];

[reverse_dir dir;
    switch (dir) 
    {
        n_obj: return s_obj;
        s_obj: return n_obj;
        e_obj: return w_obj;
        w_obj: return e_obj;
        default: rfalse;
    }
];       

[narrate_move npc direction rev_dir;
    if(npc.move_mode == TARGET_PATH or WANDER_PATH)
    {
        if(direction == u_obj) { print"^",(The)npc," "; npc.npc_walk(); " upstairs. "; } else
        if(direction == d_obj) { print"^",(The)npc," "; npc.npc_walk(); " downstairs. "; } else
        { print"^",(The)npc," "; npc.npc_walk(); print" off to the ",(name)direction,".^"; }
    }
    if(npc.move_mode == FOLLOW_PATH)
    {
        rev_dir = reverse_dir(direction);
        if(direction == u_obj) 
        { 
            print(The)npc,", following, enter";
            if(npc hasnt pluralname) { print"s";} 
            print" from downstairs.^";
            if(npc provides npc_post_follow) npc.npc_post_follow();
            rtrue;
         }
        if(direction == d_obj) 
        { 
            print(The)npc,", following, enter";
            if(npc hasnt pluralname) { print"s";} 
            print" from upstairs.^";
            if(npc provides npc_post_follow) npc.npc_post_follow();
            rtrue;
        }
        print(The)npc,", following, enter"; if(npc hasnt pluralname) { print"s";} ; print" from the ",(name)rev_dir,".^";
        if(npc provides npc_post_follow) npc.npc_post_follow();
    }
];