import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score
import matplotlib.pyplot as plt
from sklearn.impute import SimpleImputer

# 1. Chargement des datasets
#train_data = pd.read_csv('../data/data_set_0_val_manquante_converted.csv')
train_data = pd.read_csv('../data/ours/fall.csv')
test_data = pd.read_csv('immobile2-chute-ok.csv')

# Suppression de la première ligne (si elle contient des noms de colonnes supplémentaires)
train_data = train_data.iloc[1:].reset_index(drop=True)
test_data = test_data.iloc[1:].reset_index(drop=True)

# Renommez les colonnes de test_data pour qu'elles correspondent à celles de train_data
test_data.columns = train_data.columns

# Imputation des valeurs manquantes ou à zéro dans les données de test
imputer = SimpleImputer(strategy='mean')
X_test_imputed = imputer.fit_transform(test_data.drop(columns=['label']))

# Séparation des caractéristiques et des étiquettes
X_train = train_data.drop(columns=['label'])
y_train = train_data['label']

X_test = pd.DataFrame(X_test_imputed, columns=X_train.columns)
y_test = test_data['label']

# 2. Création et entraînement du modèle RandomForest
random_forest_model = RandomForestClassifier(n_estimators=100, max_depth=None, random_state=42)

# Entraînement du modèle
random_forest_model.fit(X_train, y_train)

# 3. Évaluation du modèle
# Prédictions sur les données de test
y_pred = random_forest_model.predict(X_test)

# Calcul et affichage de l'accuracy
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy :", accuracy)

# Affichage d'un rapport de classification détaillé avec zero_division
print("\nRapport de classification :\n")
print(classification_report(y_test, y_pred, zero_division=0))

# 4. Visualisation de l'importance des caractéristiques
feature_importances = random_forest_model.feature_importances_

# Affichage sous forme de barres
plt.figure(figsize=(10, 6))
plt.bar(range(len(feature_importances)), feature_importances, color='skyblue')
plt.xlabel('Caractéristiques')
plt.ylabel('Importance')
plt.title('Importance des caractéristiques dans RandomForest')
plt.show()