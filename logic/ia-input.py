import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score
import matplotlib.pyplot as plt
from sklearn.impute import SimpleImputer


# appel script ia-self.py
data = pd.read_csv('../data/ours/fall.csv')

# 1. Chargement des datasets
#train_data = pd.read_csv('../data/data_set_0_val_manquante_converted.csv')
train_data = pd.read_csv('../data/ours/fall.csv')
test_data = pd.read_csv('../data/ours/output-dataset.csv')

# Suppression de la première ligne (si elle contient des noms de colonnes supplémentaires)
train_data = train_data.iloc[1:].reset_index(drop=True)
test_data = test_data.iloc[1:].reset_index(drop=True)

# Séparation des caractéristiques et des étiquettes
X_train = train_data.drop(columns=['label'])
y_train = train_data['label']

X_test = test_data.drop(columns=['label'])
y_test = test_data['label']

# 2. Création et entraînement du modèle RandomForest
random_forest_model = RandomForestClassifier(n_estimators=100, max_depth=None, random_state=42)

# Entraînement du modèle
random_forest_model.fit(X_train, y_train)

# 3. Évaluation du modèle
# Prédictions sur les données de test
y_pred = random_forest_model.predict(X_test)

if y_pred[0] == 0:
    print("Chute détectée")
else:
    print("Pas de chute détectée")

print(y_pred)