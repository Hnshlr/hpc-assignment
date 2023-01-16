# SETUP ENVIRONMENT ...

# ... THRU GITHUB:
# ssh s388885@crescent.central.cranfield.ac.uk "cd /scratch/s388885/; ssh -T git@github.com; cd /scratch/s388885/; rm -rf hpc-assignment; git clone git@github.com:Hnshlr/hpc-assignment.git"

# ... THRU SCP:
ssh s388885@crescent.central.cranfield.ac.uk "cd /scratch/s388885/hpc-assignment/; rm -rf src/"
scp -r ../../hpc-assignment s388885@crescent.central.cranfield.ac.uk:/scratch/s388885/