import threading
import multiprocessing
import time

import os
from multiprocessing import Process


# os.system("uvicorn --host 0.0.0.0 main:app --reload");
def runHTTPServer():
    os.system("uvicorn --host 0.0.0.0 main:app --reload")


def runSerialCommunicator():
    os.system("py ServerPy.py")


def print_func(continent='Asia'):
    print('The name of continent is : ', continent)


if __name__ == "__main__":  # confirms that the code is under main function
    procs = []
    proc1 = Process(target=runHTTPServer)  # instantiating without any argument
    proc2 = Process(target=runSerialCommunicator)

    procs.append(proc1)
    procs.append(proc2)


    proc1.start()
    proc2.start()


"""
    # complete the processes
    for proc in procs:
        proc.join()
"""