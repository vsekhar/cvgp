if __name__ == "__main__":
    import sys
    buildconfig = sys.argv[1]
    newpath = sys.path[0] + "/" + buildconfig
    sys.path.insert(1,newpath)
    import vgp
    
    # Put test code here
    o = vgp.make_int_org()
    print(str(o))
    print(vgp.run_as_int(o))
    vgp.make_adf(o)
    print(str(o))
    print(vgp.run_as_int(o))