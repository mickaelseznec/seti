OWCET=owcet

.PHONY: analyse clean

analyse: heli heli.ff patmos_wcet.osx patmos_wcet/caches.xml  patmos_wcet/memory.xml  patmos_wcet/pipeline.xml
	@mkdir -p out
	$(OWCET) -f heli.ff -s patmos_wcet.osx  heli

clean:
	rm -rf out
