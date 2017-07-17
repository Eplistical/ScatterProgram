
blah = 1
blah_name = [ k for k, v in locals().items() if v is blah ][0]
print(blah_name)
