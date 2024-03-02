sources = main.c resize.c 
sources2= main1.c extractStrip.c
resize, extractstrip  : $(sources) $(sources2)
	gcc $(sources) -o resize 
	gcc $(sources2) -o extractstrip 

#tamiz :
#	rm $(objects)

