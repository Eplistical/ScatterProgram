########################################################
# class to generate paths
#
########################################################
import json
import os

class ScatterPath(object):
    """path manager class for scatter program"""
    pathdict = dict()

    def __init__(self, infile):
        self.pathdict["infile"] = os.path.abspath(infile)
        self.pathdict["jsonfile"] = self.pathdict["infile"] + '.json'
        self.pathdict["parent_dir"] = os.path.dirname(self.pathdict["infile"])
        with open(self.pathdict["jsonfile"], 'r') as f:
            self.pathdict["jsoninfo"] = json.load(f)

        jobname = self.pathdict["jsoninfo"]['rem']['jobname']['value'][0]
        fmt = self.pathdict["parent_dir"] + '/' + jobname + '.{}'
        fmt_hidden = self.pathdict["parent_dir"] + '/.' + jobname + '.{}'

        self.pathdict["outfile"] = fmt.format('out')
        self.pathdict["surface_outfile"] = fmt.format('surface.out')
        self.pathdict["simulation_outfile"] = fmt.format('simulation.out')
        self.pathdict["prepareinit_outfile"] = fmt.format('prepareinit.out')
        self.pathdict["preaparedat_outfile"] = fmt.format('preparedat.out')

        self.pathdict["logfile"] = fmt.format('log')
        self.pathdict["surface_logfile"] = fmt.format('surface.log')
        self.pathdict["simulation_logfile"] = fmt.format('simulation.log')
        self.pathdict["prepareinit_logfile"] = fmt.format('prepareinit.log')
        self.pathdict["preaparedat_logfile"] = fmt.format('preparedat.log')

        self.pathdict["initfile"] = fmt_hidden.format('init')
        self.pathdict["datfile"] = fmt_hidden.format('dat')

        self.pathdict["dyn_info_file"] = fmt.format('dyn_info.dat')

    def __getattr__(self, name):
        return self.pathdict[name]
