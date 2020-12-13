#include "FSManager.h"
typedef struct {
  String hora;
  String minut;
  String segon;
  float T;
  float RH;
  int CO2;
  int P;
} Entrada;

Entrada registro[300];

extern ESP8266WebServer server;

String oldPath="";
int pos;

char LF = 10;

void guarda(float T, float RH, int CO2, int P) {
  time_t tnow = time(nullptr);
  struct tm *timeinfo;
  char buffer [80];
  timeinfo = localtime (&tnow);
  strftime (buffer,80,"/dades/%Y-%m-%d.csv",timeinfo);
  String path(buffer);
  if (path!=oldPath && oldPath!=""){
    String txt = "HH,MM,T,RH,CO2"+String(LF);
    for (int i=0;i<pos;i++) {
      txt += registro[i].hora+":"+registro[i].minut+":"+registro[i].segon+","+String(registro[i].T)+","+String(registro[i].RH)+","+String(registro[i].CO2)+","+String(registro[i].P)+String(LF);
    }
    File file = SPIFFS.open(oldPath,"w");
    if (!file) {
      DBG_OUTPUT_PORT.println("file open failed");
    }
    file.print(txt);
    file.close();
    txt = String();      
    oldPath=path;
    pos=0;
  }
 
  strftime (buffer,80,"%H",timeinfo);
  registro[pos].hora=String(buffer);
  strftime (buffer,80,"%M",timeinfo);
  registro[pos].minut=String(buffer);
  strftime (buffer,80,"%S",timeinfo);
  registro[pos].segon=String(buffer);
  registro[pos].T=T;
  registro[pos].RH=RH;
  registro[pos].CO2=CO2;
  registro[pos].P=P;
  
  pos++; 
}

void mostra() {
  String txt = "HH:MM:SS,T,RH,CO2,P"+String(LF);
  for (int i=0;i<pos;i++) {
    txt += registro[i].hora+":"+registro[i].minut+":"+registro[i].segon+","+String(registro[i].T)+","+String(registro[i].RH)+","+String(registro[i].CO2)+","+String(registro[i].P)+String(LF);
  }
  server.send(200, "application/vnd.ms-excel", txt);
  txt = String();  
}

void printHistorics(){
  FSInfo fs_info;
  
  Dir dir = SPIFFS.openDir("/dades/");
  String output;
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    output = "&nbsp;&nbsp;&nbsp;&nbsp;";
    output += "<a href='/datadel?killfitxer=";
    output += fileName;
    output +="' style='text-decoration: none;'> &#128465; </a>";
    output += "&nbsp;&nbsp;&nbsp;&nbsp;<a href='/";
    output += fileName.substring(1);
    output +="'>";
    output += fileName.substring(7);
    output +="</a>&nbsp;&nbsp;&nbsp;&nbsp;";
    output += formatBytes(fileSize).c_str();
    output +="<br/> ";
    output +="";
    server.sendContent(output);
  }
  SPIFFS.info(fs_info);  

  output ="<br/>Usados ";
  output +=formatBytes(fs_info.usedBytes);
  output +=" de ";
  output +=formatBytes(fs_info.totalBytes);
  output +="<br/>";
  output +="Puede liberar espacio borrando archivos con el icono de la papelera &#128465;<br/>";
  server.sendContent(output);
  output = String();
}

void DataDelete() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg("killfitxer");
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (!SPIFFS.exists(path)) {
    return server.send(404, "text/plain", "FileNotFound");
  }
  SPIFFS.remove(path);
  path = String();
  server.send(200, "text/html", "<script>window.location.replace('/historic.html')</script>");
}


void grafica() {
  String out = "";
  char temp[1000];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"800\" height=\"600\">\n";
  out += "<rect width=\"800\" height=\"600\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = map(0,0,4000,550,0);
  int x = map(0,0,1440,50,800);
  int y2 = map(4000,0,4000,550,0);
  int x2 = map(1440,0,1440,50,800);
  sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, y, x2, y);
  out += temp;
  sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, y, x, y2);
  out += temp;
  x = 5;
  for (int i = 0; i< 4000; i+=1000) {
    y = map(i,0,4000,550,0);
    sprintf(temp, "<text x=\"%d\" y=\"%d\">%d</text>\n", x, y, i);
    out += temp;      
  }

  y = 575;
  for (int i = 0; i< 24; i+=3) {
    x = map(i*60,0,1440,50,800);
    sprintf(temp, "<text x=\"%d\" y=\"%d\">%d</text>\n", x, y, i);
    out += temp;      
  }


  y = map(registro[0].CO2,0,4000,550,0);
  x = map(registro[0].hora.toInt()*60+registro[0].minut.toInt(),0,1440,50,800);

  for (int i = 1; i < pos; i++) {
    y2 = map(registro[i].CO2,0,4000,550,0);
    x2 = map(registro[i].hora.toInt()*60+registro[i].minut.toInt(),0,1440,50,800);
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, y, x2, y2);
    out += temp;
    x = x2;
    y = y2;    
  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}


  
  
