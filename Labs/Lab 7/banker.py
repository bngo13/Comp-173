import numpy as np

def detect_deadlock(available, allocation, request):
    # Get Length
    n = len(allocation)

    # Make copies because idk python man ngl
    availableCopy = available.copy()
    
    # Init finished array
    finished = [False] * n
    
    # O(n^2) right here don't worry about it lol
    for _ in range(n):
        for i in range(n):
            # If not finished and req is within available, add alloc and set finished
            if not finished[i] and all(np.array(request[i]) <= availableCopy):
                availableCopy = np.add(availableCopy, allocation[i])
                finished[i] = True

    # If all have finished, no deadlock
    if all(np.array(finished) == True):
        return False, []
    # Otherwise, deadlock :(
    else:
        # Get processes that deadlocked
        ret = []
        for i in range(len(finished)):
            if not finished[i]:
                ret.append(i)
                
        # Return those deadlocked :(
        return True, ret

def read_input(file_path):
    with open(file_path, 'r') as file:
        # Get lines
        n = int(file.readline().strip())
        m = int(file.readline().strip()) # Unused cause why do this when you can just error
        
        # Get available through this bullshit called maps (we love maps)
        available = list(map(int, file.readline().strip().split()))

        # Get allocation through lines
        allocation = []
        for _ in range(n):
            allocation.append(list(map(int, file.readline().strip().split())))

        # Get allocations through more lines
        request = []
        for _ in range(n):
            request.append(list(map(int, file.readline().strip().split())))

    return n, m, available, allocation, request

def main():
    while True:
        # Get input
        file_path = input("Enter the input file path: ")
        n, m, available, allocation, request = read_input(file_path)

        # Do deadlock
        (deadlock, processes) = detect_deadlock(available, allocation, request)

        if deadlock:
            # This is literally the first time I have ever seen a join function
            print("Deadlock detected!\nProcesses: " + ' '.join(str(n) for n in processes))
        else:
            print("No deadlock detected.")

if __name__ == "__main__":
    main()
