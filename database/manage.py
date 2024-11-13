from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from connection import Entries

# Configurando a classe base e a engine do banco de dados
# Comando para rodar banco de dados:
# python -m uvicorn api:app --reload

DATABASE_URL = "postgresql+psycopg2://postgres:1234@localhost:5432/postgres"
engine = create_engine(DATABASE_URL)
Session = sessionmaker(bind=engine)

def insert(temp, oxi, bpm, ecg):
    try:
        session = Session()
        new_entry = Entries(temperature=temp, oximetry=oxi, bpm=bpm, ecg=ecg)
        session.add(new_entry)
        session.commit()
        session.close()
        return("Sucess")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")
        return(e)
    
# JSON example:
# {
#   "temperature": 0,
#   "oximetry": 0,
#   "bpm": 0,
#   "ecg": [
#     0
#   ]
# }
