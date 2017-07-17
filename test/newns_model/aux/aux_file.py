########################################################
# aux file operating functions
#
########################################################
import numpy as np

def goforward(f, start='', linenum=0):
    """move the cursor to the given line in a file
            Input:
                @f                    file object, file for search
                @start            string, the starting pattern of given line
                @linenum        int, line number to move forward
                if start & linenum both input, search the pattern first then
                move forward lines
            Return:
                #line                content of targetline
                #None                If error happens
    """
    # given start pattern
    if start.strip():
        while 1:
            try:
                line = next(f).strip()
                if line.startswith(start):
                    break
            except StopIteration:
                return None
    # move forward linenum lines
    if linenum > 0:
        for i in range(linenum-1):
            try:
                next(f)
            except StopIteration:
                return None
        try:
            line = next(f)
            return line
        except StopIteration:
            return None

    return line


########################################################
def loadmatrix(f, start='', linenum=0, printheader=False):
    """load matrix form data from a file,
         until file end or unreadable line is met
            Input:
                @f              file object
                @start          string, the starting pattern of given line
                @linenum        int, line number to move forward
                @printheader    bool, if True print out headers before matrix
                if start & linenum both input, search the pattern first then
                move forward lines
            Return:
                #data                a numpy matrix
    """
    if start.strip():
        while 1:
            try:
                line = next(f).strip()
                if printheader:
                    print(line)
                if line.startswith(start):
                    break
            except StopIteration:
                return None

    if linenum > 0:
        for i in range(linenum):
            try:
                line = next(f).strip()
                if printheader:
                    print(line)
            except StopIteration:
                return None
    # start reading
    data = []
    while 1:
        try:
            line = next(f).strip()
            if line == "":
                continue
            s = [float(j) for j in line.split()]
            data.append(s)
        except StopIteration:
            break
        except ValueError:
            break
    return np.array(data)
