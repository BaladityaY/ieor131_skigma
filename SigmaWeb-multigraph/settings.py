info = {
    "team":   "SKIGMA Solutions, Inc.",
    "members":   "Olivier, Ajay, Mattan, Phi & Aditya",
    "description":  
        """<h2><i>Revolutionizing the Skiing Industry</i></h2>""",
    "additional":
        """<h3>Instructions:</h3>
        <ul>
            <li>Begin by setting the total time (in minutes) the Resort remains open, the time it closes to new entrants, and the time skiers begin leaving. </li>
            <li>Enter the number of ski lifts at the resort. This number is capped at "3". </li>
            <li>For each ski lift, enter values such as ski lift service times and number of seats per chairlift. </li>
            <li>
            <ul>Beta distribution guidelines:
                <li> Test
                <li> Test
            </ul>
            </li>
        </ul>
        """
}

sigma = {
    "model": "SkiV8",    
    "runtime":{"name": "RUNTIME",
               "display": "Ski Resort Operating Hours (in Minutes)",
               "default": 240
              },
    "resort_name":{"name": "RESORT_NAME",
               "display": "Name of the Ski Resort",
               "default": "Enter Ski Resort Name",
              },
    "ski_lift_number":{"name": "SKI_LIFT_NUMBER",
               "display": "Number of Ski Lifts",
               "default": 3,
              },
    "parameters": [
        {   "name": "N",
            "display": "Number of ski lifts at the resort",
            "default": 3
        },
        {   "name": "CLOSE",
            "display": "Time (in minutes) when ski resort stops allowing new entrants",
            "default": 120
        },
        {   "name": "LEAVE",
            "display": "Time (in minutes) when skiers begin leaving the resort",
            "default": 180
        },
        {   "name": "SKI[0;0]",
            "display": "Time to go down slope from Ski Lift 0 to Ski Lift 0",
            "default": 10
        },
        {   "name": "SKI[0;1]",
            "display": "Time to go down slope from Ski Lift 0 to Ski Lift 1",
            "default": 12
        },
        {   "name": "SKI[0;2]",
            "display": "Time to go down slope from Ski Lift 0 to Ski Lift 2",
            "default": 8
        },
        {   "name": "SKI[1;0]",
            "display": "Time to go down slope from Ski Lift 1 to Ski Lift 0",
            "default": 8
        },
        {   "name": "SKI[1;1]",
            "display": "Time to go down slope from Ski Lift 1 to Ski Lift 1",
            "default": 6
        },
        {   "name": "SKI[1;2]",
            "display": "Time to go down slope from Ski Lift 1 to Ski Lift 2",
            "default": 6
        },
        {   "name": "SKI[2;0]",
            "display": "Time to go down slope from Ski Lift 2 to Ski Lift 0",
            "default": 6
        },
        {   "name": "SKI[2;1]",
            "display": "Time to go down slope from Ski Lift 2 to Ski Lift 1",
            "default": 6
        },
        {   "name": "SKI[2;2]",
            "display": "Time to go down slope from Ski Lift 2 to Ski Lift 2",
            "default": 6
        },
        {   "name": "PR[0;0]",
            "display": "Probability a skiier goes from ski lift 0 to ski lift 0",
            "default": .2
        },
        {   "name": "PR[0;1]",
            "display": "Probability a skiier goes from ski lift 0 to ski lift 1",
            "default": .5
        },
        {   "name": "PR[0;2]",
            "display": "Probability a skiier goes from ski lift 0 to ski lift 2",
            "default": .3
        },
        {   "name": "PR[1;0]",
            "display": "Probability a skiier goes from ski lift 1 to ski lift 0",
            "default": .3
        },
        {   "name": "PR[1;1]",
            "display": "Probability a skiier goes from ski lift 1 to ski lift 1",
            "default": .3
        },
        {   "name": "PR[1;2]",
            "display": "Probability a skiier goes from ski lift 1 to ski lift 2",
            "default": .4
        },
        {   "name": "PR[2;0]",
            "display": "Probability a skiier goes from ski lift 2 to ski lift 0",
            "default": .3
        },
        {   "name": "PR[2;1]",
            "display": "Probability a skiier goes from ski lift 2 to ski lift 1",
            "default": .3
        },
        {   "name": "PR[2;2]",
            "display": "Probability a skiier goes from ski lift 2 to ski lift 2",
            "default": .4
        },
        {   "name": "LAMBDA[0]",
            "display": "Interarrival times for chairs of ski lift 0",
            "default": .1
        },
        {   "name": "LAMBDA[1]",
            "display": "Interarrival times for chairs of ski lift 1",
            "default": .1
        },
        {   "name": "LAMBDA[2]",
            "display": "Interarrival times for chairs of ski lift 2",
            "default": .1
        },
        {   "name": "B[0]",
            "display": "Seats per chair of ski lift 0",
            "default": 2
        },
        {   "name": "B[1]",
            "display": "Seats per chair of ski lift 1",
            "default": 2
        },
        {   "name": "B[2]",
            "display": "Seats per chair of ski lift 2",
            "default": 2
        },
        {   "name": "TS[0]",
            "display": "Time it takes to go up ski lift 0",
            "default": 2
        },
        {   "name": "TS[1]",
            "display": "Time it takes to go up ski lift 1",
            "default": 3
        },
        {   "name": "TS[2]",
            "display": "Time it takes to go up ski lift 2",
            "default": 3
        },
        {   "name": "ALPHA[0]",
            "display": "Alpha value for beta distribution 0",
            "default": 2
        },
        {   "name": "ALPHA[1]",
            "display": "Alpha value for beta distribution 1",
            "default": 3
        },
        {   "name": "ALPHA[2]",
            "display": "Alpha value for beta distribution 2",
            "default": 5
        },
        {   "name": "BETA[0]",
            "display": "Beta value for beta distribution 0",
            "default": 3
        },
        {   "name": "BETA[1]",
            "display": "Beta value for beta distribution 1",
            "default": 1
        },
        {   "name": "BETA[2]",
            "display": "Beta value for beta distribution 2",
            "default": 1
        },
        {   "name": "MULT[0]",
            "display": "Multiplier for beta distribution 0",
            "default": 1
        },
        {   "name": "MULT[1]",
            "display": "Multiplier for beta distribution 1",
            "default": 1
        },
        {   "name": "MULT[2]",
            "display": "Multiplier for beta distribution 2",
            "default": 1
        },
    ]
}

rdp_enable = True

graphs = [
    {   "graph_name": "Queues vs Time",
        "x-axis": "Time",
        "x-display": "Time (minutes)",
        "y-axis": ["Q[0]", "Q[1]", "Q[2]"],
        "y-display": "Queues",
        "lines": ["Number in Queue 0", "Number in Queue 1", "Number in Queue 2"],
        "rdp_epsilon": [.6, .6, .6]
    },
]

server = {
    "hostname": "localhost",
    "port": 9000
}
