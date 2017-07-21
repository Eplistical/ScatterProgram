########################################################
# class to handle json data
#
########################################################
import json
import os
from .qctojson import datatyperange, datamodelrange

class ScatterJson(object):
    """class to retrive data in the Scatter json tree"""
    flatten_data = dict()
    def __init__(self, jsondata):
        for _, category in jsondata.items():
            for name, piece in category.items():
                if name in self.flatten_data:
                    raise KeyError('paraname ' + name + ' appeared more than once')
                self.flatten_data[name] = self._convertdata(piece)

    def __getattr__(self, name):
        return self.flatten_data[name]

    @staticmethod
    def _convertdata(piece):
        """convert data piece to correct type"""
        assert (piece['datatype'] in datatyperange)
        assert (piece['datamodel'] in datamodelrange)
        convert_table = {
                'b' : bool,
                'i' : int,
                'd' : float,
                's' : str,
                'S' : str,
                }
        convert_func = convert_table[piece['datatype']]

        if piece['datamodel'] == 's':
            return convert_func(piece['value'][0])
        elif piece['datamodel'] == 'v':
            return tuple(convert_func(k) for k in piece['value'])
        elif piece['datamodel'] == 'b':
            return tuple(tuple(convert_func(k) for k in v) for v in piece['value'])

# END
