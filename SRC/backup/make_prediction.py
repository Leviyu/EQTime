#!/usr/bin/python3


import sys
import pandas as pd 
import matplotlib as plt
import numpy as np
import scipy as sp
import IPython
from IPython import display
import sklearn
import random
import time

import warnings
warnings.filterwarnings("ignore")
#Common Model Algorithms
from sklearn import svm, tree, linear_model, neighbors, naive_bayes, ensemble, discriminant_analysis, gaussian_process
##from xgboost import XGBClassifier
import pickle
#Common Model Helpers
from sklearn import preprocessing
from sklearn.preprocessing import OneHotEncoder, LabelEncoder
from sklearn import feature_selection
from sklearn import model_selection
from sklearn import metrics
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC, LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn import tree
from sklearn.tree import DecisionTreeClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.gaussian_process import GaussianProcessClassifier
from sklearn.gaussian_process.kernels import RBF
from sklearn.ensemble import AdaBoostClassifier
import time
from sklearn.model_selection import KFold, cross_val_score
from sklearn.metrics import precision_recall_fscore_support
from sklearn.ensemble import BaggingClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.ensemble import VotingClassifier
import lightgbm as lgb
from sklearn.model_selection import KFold, StratifiedKFold
from sklearn.metrics import mean_squared_error

#Visualization
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.pylab as pylab
import seaborn as sns
from pandas.tools.plotting import scatter_matrix



inputfile = sys.argv[1]
headerfile = "./header"
header = pd.read_csv(headerfile,sep=' ')


data = pd.read_csv(inputfile,sep=r'\s+',names = header.columns)
#print(data.head().T)

data.NET.fillna("NNN",inplace=True)
#print(data.NET)

# drop columns that is obviously not correlated with the quality of the record
drop_list = [
"STA",
'PREM',
'AMP',
'TAG1',
'TAG2',
'TAG3',
'TAG4',
'TAG5',
'TAG6',
'TAG7',
'ONSET',
'ENDSET',
'PRE3T',
'BAK3T',
"EQ_NAME_STA_PHASE",
"AZ",
'COMP'
]

for col in drop_list:
    if col in data.columns:
        data = data.drop(col,axis=1)

# drop columns that has only one value
#for col in data.columns:
    #uniquenum = data[col].unique().shape[0]
    #if uniquenum == 1:
        #data = data.drop(col,axis=1)
        

current_phase_name = data.head(1)['PHASE'].values[0]
print("current_phase_name is ", current_phase_name)

# Backup original data
data_raw = data.copy()

data.loc[data.QUALITY>=1,'CODE_DECISION'] = 1
data.loc[data.QUALITY<1,'CODE_DECISION'] = 0


# Classify different features into numerical and categorial features
numerical_feature = [
'DIST', 'CCC1', 'SNRAVE', 'DT',
'STRETCHC', 'STRETCHF', 'MISFITSIG', 'WEIGHT', 'TSTARF',
'TSTARC', 'CCC2', 'MISFITPRE', 'MISFITBAK', 'GRECORD', 'GMISFIT',
'POLARITY', 'PRE2T', 'BAK2T',
'ONE_PERIOD',
'EQ_DEP','EQ_MAG',
'STA_LAT','STA_LON','BAZ','EQ_LON','EQ_LAT',
'GSEW','SNR_PEAK'
                    ]
categorial_feature = [
'NET', 
'POLARFLAG', 
'QUALITY',
'POLAR_RIGHT',
'TRAFFIC_IN_NOISE',
'TRAFFIC',
'CODE_DECISION'
    ]
dummy_feature = [
    'PHASE'
]
print(data.columns)

print("total feature num is {0} numerical feature num is {1} categorial feature num is {2}".format(
data.shape[1],len(numerical_feature),len(categorial_feature)))

feat_resi = [x for x in data.columns if x not in numerical_feature if x not in categorial_feature
            if x not in dummy_feature]

# all_feat = [x for x in numerical_feature or x in dummy_feature or x in categorial_feature]
all_feat = numerical_feature+ categorial_feature
# all_feat
for col in all_feat:
    if col not in data.columns:
        all_feat.remove(col)
        if col in numerical_feature:
            numerical_feature.remove(col)
        if col in categorial_feature:
            categorial_feature.remove(col)

print(all_feat,len(all_feat))
print(feat_resi)



# For categorial features, we directly label encode them

from sklearn.preprocessing import OneHotEncoder
enc = OneHotEncoder(handle_unknown='ignore',categorical_features=[0])
label = LabelEncoder()    

# for categorial feature, we use label endocing
for feature in categorial_feature:
    print("--> Work on encoding for ", feature)
    #label.fit(data[feature])
    # save encoder
    label.classes_ = np.load(feature+'.encoder.npy')
    data[feature] = label.transform(data[feature])


#print(data.PHASE)

    
# For dummy variable we use onehotencoding
#print(dummy_feature)
new_dummy = ['S', 'SS', 'SSS', 'ScS', 'ScSScS', 'Sdiff']
#for feature in dummy_feature:
    #print("--> work on dummy feature", feature)
    #ff = pd.get_dummies(data[feature])
    #for f in ff.columns:
        #new_dummy.append(f)
    #data = pd.concat([data,ff],axis=1)
    #data.drop(feature,inplace=True,axis=1)

#all_feat.remove('PHASE')
for col in new_dummy:
    all_feat.append(col)


for col in new_dummy:
    if col == current_phase_name:
        #print( "->> find current phase name")
        data[col] = 1
    else:
        data[col] =0
    #data[col]


# Remove outlier
feat = 'SNRAVE'
value = 30
data.loc[data[feat]>value,feat] = value

feat = 'MISFITSIG'
value = 10
data.loc[data[feat]>value,feat] = value

feat = 'MISFITPRE'
value = 10
data.loc[data[feat]>value,feat] = value

feat = 'MISFITBAK'
value = 10
data.loc[data[feat]>value,feat] = value

feat = 'GMISFIT'
value = 50
data.loc[data[feat]>value,feat] = value


feat = 'PRE2T'
value = 10
data.loc[data[feat]>value,feat] = value

feat = 'BAK2T'
value = 10
data.loc[data[feat]>value,feat] = value


#print(data[all_feat])
model_list = [0 , 1,  2,  3,  4]
#model_list = [1]

final_pred = np.zeros(data.shape[0])

final_pred_int = pd.DataFrame(columns=['pred'])
init_value = np.zeros(data.shape[0])
final_pred_int['pred'] = init_value

for model_num in model_list:
    model = pickle.load(open("./lightgbm_EW."+str(model_num)+".pkl",'rb'))
    pred = model.predict(data[all_feat],num_iteration=5000)
    #final_pred += np.array(pred) / len(model_list)
    final_pred_int['pred'] += np.array( pred) / len(model_list)
    #print([int(x/0.5) for x in pred])


final_pred_int['pred'] = [ int( (x)/0.5) for x in final_pred_int['pred']]

#final_pred_int = [ int(x/0.5) for x in final_pred]
#print(final_pred_int)
#print(sum(final_pred_int))
#print(final_pred_int.pred)
#print(final_pred_int.pred.sum())

data['pred'] = final_pred_int['pred']
data['ML_FLAG'] = ( data['pred'] == data['CODE_DECISION'] ).astype(int)
final_pred_int['ML_FLAG'] = data['ML_FLAG']
final_pred_int.to_csv("./pred",index=False,sep = " ")






