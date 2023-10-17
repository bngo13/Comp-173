

fn main() {
    let mut vec = vec![
        vec![0, 0, 12, 0, 0, 0, 0], 
        vec![1, 2, 4, 0, 0, 0, 0],
        vec![2, 3, 1, 0, 0, 0, 0],
        vec![3, 4, 2, 0, 0, 0, 0],
    ];
    FCFS(&mut vec);
    println!("{:?}", vec);
}

fn FCFS(processes: &mut Vec<Vec<i32>>) {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
    // pid arrival burst already filled
    
    // Calculate start time
    let mut total_time = 0;
    for process in &mut *processes {
        process[3] = total_time;
        total_time += process[2];
    }

    // Calculate End Time
    for process in &mut *processes {
        process[4] = process[3] + process[2];
    }

    // Calculate Running Time
    for process in &mut *processes {
        process[5] = process[4] - process[3];
    }

    // Caluclate Wait Time
    for process in &mut *processes {
        process[6] = process[3] - process[1];
    }
 
}

fn SJF(processes: &mut Vec<Vec<i32>>) {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
}

fn RR(processes: &mut Vec<Vec<i32>>, quantum: i32) {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
}
