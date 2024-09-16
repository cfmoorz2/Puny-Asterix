Attribute touched;
Constant TARGET_PATH = 2;
Constant WANDER_PATH = 1;
Constant FOLLOW_PATH = 3;


Array queue --> 64;

Class Room 
    with hop,
         hop_direction,
         each_turn [obj direction one_out npc;
            objectloop (direction in compass)
            {
                one_out = real_location.(direction.door_dir);
                !print"one_out start = ",(name)one_out,"^";
                if (one_out ofclass myDoor)
                {
                    one_out = one_out.door_to(direction.door_dir);
                    !print"one_out DOOR = ",(name)one_out,"^";
                } 
                if (one_out ofclass Room)
                {
                    objectloop (obj in one_out)
                    {
                        if(obj has animate && obj.hide == false)
                        {
                            print"^",(The)obj," is ";
                            if(direction == u_obj) "up above. ";
                            if(direction == d_obj) "down below. ";
                            print"off to the ",(name)direction,".^";
                        }
                    }
                }
            }
            objectloop (npc && npc ofclass Mover)
            {
                npc.hide = false;
                !print"giving hide = false to ",(name)npc,"^";
            }
         ],
	has light;

Class myDoor 
    with hop,
        hop_direction,
        !npc_open [;
        !],
    has door;

Class Mover 
    with move_mode, !0 = stationary, 1 = wander, 2 = path
        target_room,
        npc_walk [; print"heads";],
        npc_follow [; print"enters";],
        npc_avoid 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0,
        npc_wander_delay,
        npc_last_wander,
        npc_arrived [;],
        npc_post_move [; rfalse;],
        daemon [ x y i;
            if(self.move_mode == TARGET_PATH)
			{
				path_move(parent(self), self.target_room);
			}
            if(self.move_mode == FOLLOW_PATH)
            {
                self.target_room = real_location;
                !print"moving to ",(name)self.target_room,"^";
                path_move(parent(self), self.target_room);
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
                    if(TestScope(self)) print(The)self," picks up ",(a)i,".^";
                }
            }
            rfalse;
        ],
        hide false,
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
            !print(name)obj," has touched.^";
            !print"queueing ",(name)obj," at index ",i,"^";
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
    !print"start room = ",(name)start_room,", end_room = ",(name)end_room,"^";
    obj = start_room;
    while (obj)
    {
        obj = dequeue();
        !print"here obj = ",(name)obj,"^";
        if(obj == end_room) rtrue;
        eval_node(obj);
    }
];

[eval_node current_node direction k temp;
    !print"evalling node ",(name)current_node,"^";
    objectloop (direction in compass)
    {
        k = current_node.(direction.door_dir);
        !print"k = ",(name)k,"^";
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
        !print"prev = ",(name)prev,", prev.hop = ",(name)prev.hop,", direction = ",(name)prev.hop_direction,"^";
        if(prev.hop == parent(self))
        {
            final = prev.hop_direction;
            !print"final = ",(name)final,"^";
            way = parent(self).(final.door_dir);
            if (way has door)
            {
                if (way provides npc_open) 
                {
                    way.npc_open(self);
                }
                else
                {
                if (TestScope(self, player) && way hasnt open) print(The)self," pushes open the door.^";
                give way open;
                give way ~locked;
                }
                !print"pre_way = ",(name)way,"^";
                move way to parent(self);
                temp = way;
                way = way.door_to(final.door_dir);
                !print"here way = ",(name)way,"^";
                !print"here parent(self)= ",(name)parent(self),"^";
                remove temp;
                MoveFloatingObjects();
            }
            !print"^[moving ",(name)self," to the ",(name)final,"]^";
            eval_one_away(self, final);
            if (TestScope(self, player) && self.move_mode == TARGET_PATH) { self.hide = true; narrate_move(self, final); }
            move self to way;
            scope_modified = true;
            if (TestScope(self, player) && self.move_mode == FOLLOW_PATH) { self.hide = true; narrate_move(self, final); }
            if (self provides npc_post_move) self.npc_post_move();
            if (TestScope(self, player) && self.hide == false) 
            {
                print"^",(name)self," is here.^";
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
    objectloop(direction in compass) 
    {
        k = location.(direction.door_dir);
        if (npc in k) 
        {
            if (direction == d_obj) 
            {
                print"^",(The)npc," is down below heading ";
                if (final == d_obj) "downstairs. ";
                if (final == u_obj) "upstairs. ";
                "to the ",(name)final,".";
            }
            if (direction == u_obj) 
            {
                print"^",(The)npc," is up above heading ";
                if (final == d_obj) "downstairs. ";
                if (final == u_obj) "upstairs. ";
                "to the ",(name)final,".";
            }
            
            print"^",(The)npc," is off to the ",(name)direction;
            if (final == d_obj) " heading downstairs. ";
            if (final == u_obj) " heading upstairs. ";
            " heading to the ",(name)final,".";
        }
    }
];

[wander_move npc_at test_room direction i x;
    npc_at = parent(self);
    !print"avoid direction = ",(name)self.npc_last_wander,"^";
    objectloop (direction in compass)
    {
        !if (direction == self.npc_last_wander) continue;
        test_room = npc_at.(direction.door_dir);
        !print"test_room = ",(name)test_room,"^";
        if (~~test_room ofclass Room) continue;
        if(assess_no_go(self, test_room)) continue;
        !print"test_room HERE = ",(name)test_room,"^";
        i++;
    }
    !print"i == ",i,"^";
    if (i ==0 ) rfalse;
    if (i == 1) self.npc_last_wander = 0;
    i = 0;
    !print"after first pass avoid direction = ",(name)self.npc_last_wander,"^";
    objectloop (direction in compass)
    {
        if (direction == self.npc_last_wander) continue;
        test_room = npc_at.(direction.door_dir);
        !print"test_room = ",(name)test_room,"^";
        if (~~test_room ofclass Room) continue;
        if(assess_no_go(self, test_room)) continue;
        !print"test_room HERE = ",(name)test_room,"^";
        i++;
    }
    !print"after second check i = ",i,"^";
    x = random(i);
    !print"random x = ",x,"^";
    i = 0;
    objectloop (direction in compass)
    {
        if (direction == self.npc_last_wander) continue;
        test_room = npc_at.(direction.door_dir);
        !print"test_room 2 = ",(name)test_room,"^";
        if (~~test_room ofclass Room) continue;
        if(assess_no_go(self, test_room)) continue;
        !print"test_room 2 HERE = ",(name)test_room,"^";
        i++;
        if (i == x) 
        { 
            !print"[moving ",(name)self," to ",(name)test_room,"]^";
            eval_one_away(self, direction);
            if (TestScope(self, player)) { self.hide = true; narrate_move(self, direction); }
            move self to test_room;
            scope_modified = true;
            if (TestScope(self, player) && self.move_mode == FOLLOW_PATH) { self.hide = true; narrate_move(self, direction); }
            self.npc_last_wander = reverse_dir(direction);
            !print"after final pass avoid direction = ",(name)self.npc_last_wander,"^";
            if (TestScope(self, player)) print"^",(name)self," is here.^";
           rtrue;
        }
    }
];

[assess_no_go npc test_room i;
    !print"in assess_no_go room = ",(name)test_room,"^";
    for ( i=0: i < 32: i++)
    {
        !print"avoiding ",(name)npc.&npc_avoid-->i,"^";
        if (npc.&npc_avoid-->i == test_room) rtrue; 
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
        if(direction == u_obj) { print"^",(name)npc," "; npc.npc_walk(); " upstairs."; }
        if(direction == d_obj) { print"^",(name)npc," "; npc.npc_walk(); " downstairs."; }
        print"^",(name)npc," "; npc.npc_walk(); print" off to the ",(name)direction,".^";
    }
    if(npc.move_mode == FOLLOW_PATH)
    {
        rev_dir = reverse_dir(direction);
        if(direction == u_obj) { print"^",(name)npc,", following, enters from from downstairs.^"; }
        if(direction == d_obj) { print"^",(name)npc,", following, enters from upstairs.^"; }
        print"^",(name)npc,", following, "; self.npc_follow(); print" from the ",(name)rev_dir,".^";
        if(npc provides npc_post_follow) npc.npc_post_follow();
    }
];