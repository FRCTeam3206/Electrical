<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.7.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="Raj">
<packages>
<package name="QFN12">
<description>PQFN12 3mm x 3mm</description>
<smd name="P$13" x="0" y="0" dx="1.65" dy="1.65" layer="1"/>
<smd name="P$11" x="0" y="1.4" dx="0.25" dy="0.7" layer="1"/>
<smd name="P$12" x="-0.5" y="1.4" dx="0.25" dy="0.7" layer="1"/>
<smd name="P$10" x="0.5" y="1.4" dx="0.25" dy="0.7" layer="1"/>
<smd name="P$4" x="-0.5" y="-1.4" dx="0.25" dy="0.7" layer="1"/>
<smd name="P$5" x="0" y="-1.4" dx="0.25" dy="0.7" layer="1"/>
<smd name="P$6" x="0.5" y="-1.4" dx="0.25" dy="0.7" layer="1"/>
<smd name="P$8" x="1.4" y="0" dx="0.7" dy="0.25" layer="1"/>
<smd name="P$9" x="1.4" y="0.5" dx="0.7" dy="0.25" layer="1"/>
<smd name="P$7" x="1.4" y="-0.5" dx="0.7" dy="0.25" layer="1"/>
<smd name="P$2" x="-1.4" y="0" dx="0.7" dy="0.25" layer="1"/>
<smd name="P$1" x="-1.4" y="0.5" dx="0.7" dy="0.25" layer="1"/>
<smd name="P$3" x="-1.4" y="-0.5" dx="0.7" dy="0.25" layer="1"/>
<wire x1="1.5" y1="-1.5" x2="-1.5" y2="-1.5" width="0" layer="20"/>
<wire x1="-1.5" y1="-1.5" x2="-1.5" y2="1.25" width="0" layer="20"/>
<wire x1="-1.5" y1="1.25" x2="-1.25" y2="1.5" width="0" layer="20"/>
<wire x1="-1.25" y1="1.5" x2="1.5" y2="1.5" width="0" layer="20"/>
<wire x1="1.5" y1="1.5" x2="1.5" y2="-1.5" width="0" layer="20"/>
<wire x1="0.7" y1="1.5" x2="1.5" y2="1.5" width="0.127" layer="21"/>
<wire x1="1.5" y1="1.5" x2="1.5" y2="0.7" width="0.127" layer="21"/>
<wire x1="1.5" y1="-0.7" x2="1.5" y2="-1.5" width="0.127" layer="21"/>
<wire x1="1.5" y1="-1.5" x2="0.7" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-0.7" y1="-1.5" x2="-1.5" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-1.5" y1="-1.5" x2="-1.5" y2="-0.7" width="0.127" layer="21"/>
<wire x1="-1.5" y1="0.7" x2="-1.5" y2="1.2" width="0.127" layer="21"/>
<wire x1="-1.5" y1="1.2" x2="-1.2" y2="1.5" width="0.127" layer="21"/>
<wire x1="-1.2" y1="1.5" x2="-0.7" y2="1.5" width="0.127" layer="21"/>
<text x="-1.6" y="2.3" size="0.8128" layer="25" font="vector">&gt;NAME</text>
<text x="-2.5" y="-3" size="0.8128" layer="27" font="vector">LTC2945</text>
</package>
</packages>
<symbols>
<symbol name="LTC2945">
<pin name="VDD" x="-17.78" y="20.32" length="middle"/>
<pin name="INTVCC" x="-17.78" y="15.24" length="middle"/>
<pin name="ADR1" x="-17.78" y="10.16" length="middle"/>
<pin name="ADR0" x="-17.78" y="5.08" length="middle"/>
<pin name="ADIN" x="-17.78" y="0" length="middle"/>
<pin name="GND" x="-17.78" y="-5.08" length="middle"/>
<pin name="SCL" x="17.78" y="-5.08" length="middle" rot="R180"/>
<pin name="SDAI" x="17.78" y="0" length="middle" rot="R180"/>
<pin name="SDAO" x="17.78" y="5.08" length="middle" rot="R180"/>
<pin name="ALERT" x="17.78" y="10.16" length="middle" rot="R180"/>
<pin name="SENSE-" x="17.78" y="15.24" length="middle" rot="R180"/>
<pin name="SENSE+" x="17.78" y="20.32" length="middle" rot="R180"/>
<pin name="GND2" x="0" y="-12.7" length="middle" rot="R90"/>
<wire x1="-12.7" y1="22.86" x2="-12.7" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-7.62" x2="12.7" y2="-7.62" width="0.254" layer="94"/>
<wire x1="12.7" y1="-7.62" x2="12.7" y2="22.86" width="0.254" layer="94"/>
<wire x1="12.7" y1="22.86" x2="-12.7" y2="22.86" width="0.254" layer="94"/>
<text x="-12.7" y="27.94" size="2.54" layer="95" font="vector">&gt;NAME</text>
<text x="-12.7" y="-20.32" size="2.54" layer="96" font="vector">&gt;VALUE</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="LTC2945">
<description>LTC2945
Wide Range I2C Power Monitor</description>
<gates>
<gate name="G$1" symbol="LTC2945" x="0" y="0"/>
</gates>
<devices>
<device name="" package="QFN12">
<connects>
<connect gate="G$1" pin="ADIN" pad="P$4"/>
<connect gate="G$1" pin="ADR0" pad="P$3"/>
<connect gate="G$1" pin="ADR1" pad="P$2"/>
<connect gate="G$1" pin="ALERT" pad="P$9"/>
<connect gate="G$1" pin="GND" pad="P$5"/>
<connect gate="G$1" pin="GND2" pad="P$13"/>
<connect gate="G$1" pin="INTVCC" pad="P$1"/>
<connect gate="G$1" pin="SCL" pad="P$6"/>
<connect gate="G$1" pin="SDAI" pad="P$7"/>
<connect gate="G$1" pin="SDAO" pad="P$8"/>
<connect gate="G$1" pin="SENSE+" pad="P$11"/>
<connect gate="G$1" pin="SENSE-" pad="P$10"/>
<connect gate="G$1" pin="VDD" pad="P$12"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="Raj" deviceset="LTC2945" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="83.82" y="55.88"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
