import pandas as pd
import matplotlib.pyplot as plt
import pyrebase

def initDatabase(db):
  current = []
  voltage = []

  try:
    voltage_db = db.child("voltage").get()
    current_db = db.child("current").get()
  except Exception as e:
    print(e)
    return 
  
  for ddp in voltage_db.each():
    voltage.append(ddp.val())

  for ddp in current_db.each():
    parsedCurrent = (-ddp.val() + 1.7)/10**5
    current.append(parsedCurrent)
  
  data = {'Voltage': voltage, 'Current': current}
  return data

def exportCSV(data):
  csvFile = pd.DataFrame(data, columns=['Voltage', 'Current'])
  csvFile.to_csv('export_dataframe.csv', index = False, header=True)

  print('Os dados foram exportados com sucesso!')
  return

def getVoltametricGraph(data):
  plt.title('Voltametria cíclica')
  plt.xlabel('Tensão (Volt)')
  plt.ylabel('Corrente (Amper)')
  plt.scatter(data['Voltage'], data['Current'])
  plt.savefig('voltametricData.png', format='png')
  plt.show()

  print('Você poderá ver o gráfico no mesmo diretório que rodou a aplicação')
  return

def startNewMeasurement(db):
  db.child('measurement').update({'measurement': True })
  return

def endMeasurement(db):
  db.child('measurement').update({'measurement': False })
  return

if __name__ == '__main__':
  print('Sistema de análise e Medição de sal na água')
  #inicializando Firebase
  firebaseConfig={
    "apiKey": "your api key",
    "authDomain": "your authDomain",
    "databaseURL": "your database",
    "projectId": "your project id",
    "storageBucket": "storageBucket",
    "messagingSenderId": "messagingSenderId",
    "appId": "appId",
    "measurementId": "measurementId"
  }

  firebase = pyrebase.initialize_app(firebaseConfig)
  db = firebase.database()

  database = initDatabase(db)

  while True:
    option = input('''
    Digite as informações desejadas: 
    [1] - exportar arquivo csv
    [2] - ver o gráfico da voltametria cíclica
    [3] - inicializar uma nova medição
    [4] - encerrar a medição
    [5] - sair da aplicação
    ''')
  
    if option == '1':
      exportCSV(database)
    elif option == '2':
      getVoltametricGraph(database)
    elif option == '3':
      startNewMeasurement(db)
    elif option == '4':
      endMeasurement(db)
    elif option == '5':
      print('Voce saiu da aplicação')
      break