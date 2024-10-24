insert:
            if (~~second ofclass DumbwaiterShaft) "It just floats back out. ";
            if (balloon.tied_to == player)
            {
                balloon.tied_to = 0;
                move balloon to second_floor_dumbwaiter;
                scope_modified = true;
                if (real_location == hallway_2_2) "You put the balloon in the dumbwaiter. It floats gently upward and 
                bounces on the roof of the shaft here. ";
                "You release the balloon into the dumbwaiter. It rises up and out of sight. ";
            }
            if (self.tied_to ofclass Floatable)
            {
                move balloon.tied_to to second_floor_dumbwaiter;
                if (real_location == hallway_2_2) "You put the balloon in the dumbwaiter. It floats gently upward and 
                    bounces on the roof of the shaft here, ",(the)balloon.tied_to," dangling from it.";
                    "You release the balloon into the dumbwaiter. It rises up and out of sight, carrying ",(the)balloon.tied_to," with it.";
            }
            <<insert balloon.tied_to second>>;
            ];