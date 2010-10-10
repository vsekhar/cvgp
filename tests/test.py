from time import sleep
from threading import Event, Thread
 
class RepeatTimer(Thread):
    def __init__(self, interval, target, iterations=0, args=[], kwargs={}, name=''):
        Thread.__init__(self, name=name)
        self.interval = interval
        self.target = target
        self.iterations = iterations
        self.args = args
        self.kwargs = kwargs
        self.finished = Event()
 
    def run(self):
        count = 0
        while not self.finished.is_set() and (self.iterations <= 0 or count < self.iterations):
            self.finished.wait(self.interval)
            if not self.finished.is_set():
                self.target(*self.args, **self.kwargs)
                count += 1
 
    def cancel(self):
        self.finished.set()

def blah():
	print('blah')

if __name__ == "__main__":
	import sys
	buildconfig = sys.argv[1]
	newpath = sys.path[0] + "/" + buildconfig
	sys.path.insert(1,newpath)
	import libvgp as vgp

	# Put test code here
	o = vgp.make_int_org()
	print(str(o))
	print(vgp.run_as_int(o))

	rt = RepeatTimer(interval=5, target=blah)
	rt.start()
	print("Creating population")
	vgp.memtest(100,0)
	print("Running memtest:")
	vgp.memtest(0,1000000)
	print("Running memtest_mt:")
	vgp.memtest_mt(0,1000000)
	rt.cancel()

