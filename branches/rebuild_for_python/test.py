if __name__ == "__main__":
    import sys
    buildconfig = sys.argv[1]
    newpath = sys.path[0] + "/" + buildconfig
    sys.path.insert(1,newpath)
    import vgp
    
    # Put test code here
    while 1:
        s = str(vgp.make_int_tree())
        if "vgp_adapter_" in s:
            print(s)
            break
