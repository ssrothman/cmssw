from coffea.nanoevents import NanoEventsFactory, NanoAODSchema
import uproot
import awkward as ak
import numpy as np
import hist

f = uproot.open("ntuple.root")
AE = NanoEventsFactory.from_root('ntuple_tfile.root', schemaclass = NanoAODSchema, treepath = f.keys()[1]).events()
t0 = NanoEventsFactory.from_root('ntuple_tfile.root', schemaclass = NanoAODSchema, treepath = f.keys()[3]).events()

totSimE_AE = ak.sum(AE.tc.simenergy, axis=-1)
totSimE_T0 = ak.sum(t0.tc.simenergy, axis=-1)
print("total sim energy AE/threshold0")
print(totSimE_AE/totSimE_T0)
print()

totRecoE_AE = ak.sum(AE.tc.energy, axis=-1)
totRecoE_T0 = ak.sum(t0.tc.energy, axis=-1)
print("total reco energy AE/threshold0")
print(totRecoE_AE/totRecoE_T0)
print()


#find IDs in both
AEinT0 = np.isin(AE.tc.id[0], t0.tc.id[0])
AEidx = np.argsort(AE.tc.id[0, AEinT0])

T0inAE = np.isin(t0.tc.id[0], AE.tc.id[0])
T0idx = np.argsort(t0.tc.id[0, T0inAE])

assert np.all(AE.tc.id[0, AEinT0][AEidx] == t0.tc.id[0, T0inAE][T0idx])

simE_AE = AE.tc.simenergy[0][AEinT0][AEidx]
simE_T0 = t0.tc.simenergy[0][T0inAE][T0idx]
print("simenergy is unaffected by AE:")
print("mean (AE simenergy - Threshold0 simenergy):", np.mean(simE_AE-simE_T0))
print("stdev (AE simenergy - Threshold0 simenergy):", np.std(simE_AE-simE_T0))
print()

H = hist.Hist(
  hist.axis.Regular(10, 1e-7, 1, name='simenergy', transform=hist.axis.transform.log),
  hist.axis.Regular(10, 1e-7, 1, name='energy', transform=hist.axis.transform.log),
  hist.axis.Regular(10, ak.min(np.abs(AE.tc.eta)), ak.max(AE.tc.eta), name='abseta'),
  hist.axis.StrCategory(['AE in T0', 'AE only', 'T0 only', 'T0 in AE'], name='label')
)

print("Energy as (tcs from both AE and T0) + (tcs in one but not the other) = (total)")
for i in range(10):
  AEinT0 = np.isin(AE.tc.id[i], t0.tc.id[i])
  T0inAE = np.isin(t0.tc.id[i], AE.tc.id[i])

  E = AE.gen.energy[i,0]

  H.fill(
    simenergy = (AE.tc.simenergy[i][AEinT0]/E),
    energy = (AE.tc.energy[i][AEinT0]/E),
    abseta = (np.abs(AE.tc.eta[i][AEinT0])),
    label = 'AE in T0'
  )

  H.fill(
    simenergy = (AE.tc.simenergy[i][~AEinT0]/E),
    energy = (AE.tc.energy[i][~AEinT0]/E),
    abseta = (np.abs(AE.tc.eta[i][~AEinT0])),
    label = 'AE only'
  )

  H.fill(
    simenergy = (t0.tc.simenergy[i][T0inAE]/E),
    energy = (t0.tc.energy[i][T0inAE]/E),
    abseta = (np.abs(t0.tc.eta[i][T0inAE])),
    label = 'T0 in AE'
  )

  H.fill(
    simenergy = (t0.tc.simenergy[i][~T0inAE]/E),
    energy = (t0.tc.energy[i][~T0inAE]/E),
    abseta = (np.abs(t0.tc.eta[i][~T0inAE])),
    label = 'T0 only'
  )

  print("event %d:"%i)
  print("\tAE energy = %0.3f + %0.3f = %0.3f"%(np.sum(AE.tc.energy[i][AEinT0]),
                                             np.sum(AE.tc.energy[i][~AEinT0]),
                                             np.sum(AE.tc.energy[i])))
  print("\tT0 energy = %0.3f + %0.3f = %0.3f"%(np.sum(t0.tc.energy[i][T0inAE]),
                                             np.sum(t0.tc.energy[i][~T0inAE]),
                                             np.sum(t0.tc.energy[i])))

import matplotlib.pyplot as plt

AEinT0_H = H[:, :, :, 'AE in T0']
AEonly_H = H[:, :, :, 'AE only']
T0inAE_H = H[:, :, :, 'T0 in AE']
T0only_H = H[:, :, :, 'T0 only']

fig, ((ax00, ax01), (ax10, ax11)) = plt.subplots(2, 2)

cmap = 'Reds'

w, x, y = AEonly_H.project('abseta', 'simenergy').to_numpy()
ax00.set_title("simHits only in AE")
ax00.pcolormesh(x, y, w.T, cmap=cmap)
ax00.set_yscale('log')
ax00.set_ylabel("simenergy")
ax00.set_xlabel("|eta|")

w, x, y = T0only_H.project('abseta', 'simenergy').to_numpy()
ax10.set_title("simHits only in T0")
ax10.pcolormesh(x, y, w.T, cmap=cmap)
ax10.set_yscale('log')
ax10.set_ylabel("simenergy")
ax10.set_xlabel("|eta|")

w, x, y = AEinT0_H.project('abseta', 'simenergy').to_numpy()
ax01.set_title("simHits in both")
ax01.pcolormesh(x, y, w.T, cmap=cmap)
ax01.set_yscale('log')
ax01.set_ylabel("simenergy")
ax01.set_xlabel("|eta|")

plt.tight_layout()

plt.savefig("test.png", format='png', bbox_inches='tight')
plt.show()
