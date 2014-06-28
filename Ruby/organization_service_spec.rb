location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'repository'
require 'organization_service'
require 'conference_service'
require 'organization'
require 'organization_conference'
require 'conference'


db = SQLite3::Database.new 'ncfo_test.db'

db.results_as_hash  = true
db.type_translation = true


describe 'OrganizationService' do
  describe '#get' do
    before :each do
      @repository           = Repository.new db
      conference_service    = ConferenceService.new @repository, nil
      @organization_service = OrganizationService.new @repository, conference_service

      @repository.start_transaction
    end

    after :each do
      @repository.cancel_transaction
    end

    it 'should return a organization with values from the database record' do
      db.execute 'insert into organizations_t values (1, "orgname", "ON", 7)'

      organization = @organization_service.get 1

      expect( organization ).to_not be_nil
      expect( organization ).to     be_a   Organization

      expect( organization.organization_id ).to eq 1
      expect( organization.name            ).to eq 'orgname'
      expect( organization.abbreviation    ).to eq 'ON'
      expect( organization.season          ).to eq 7
    end

    it 'should return nil if the organization is not found' do
      organization = @organization_service.get 1

      expect( organization ).to be_nil
    end

    it 'should return a organization with organization conferences when organization conferences are available for that organization' do
      db.execute 'insert into organizations_t values (1, "orgname", "ON", 7)'
      db.execute "insert into organization_conferences_t values (1, 3)"
      db.execute "insert into organization_conferences_t values (1, 5)"

      organization = @organization_service.get 1

      expect( organization ).to_not be_nil
      expect( organization ).to     be_a   Organization

      expect( organization.conferences ).to_not be_nil
      expect( organization.conferences ).to     be_a   Array

      expect( organization.conferences.length ).to eq 2

      expect( organization.conferences[0].organization_id ).to eq 1
      expect( organization.conferences[0].conference_id   ).to eq 3

      expect( organization.conferences[1].organization_id ).to eq 1
      expect( organization.conferences[1].conference_id   ).to eq 5
    end

    it 'should return a organization without organization conferences when organization conferences are not available for that organization' do
      db.execute 'insert into organizations_t values (1, "orgname", "ON", 7)'

      organization = @organization_service.get 1

      expect( organization ).to_not be_nil
      expect( organization ).to     be_a   Organization

      expect( organization.conferences ).to be_nil
    end

    it 'should return a organization with organization conferences that has a conference if that conference is available' do
      db.execute 'insert into organizations_t values (1, "orgname", "ON", 7)'
      db.execute "insert into organization_conferences_t values (1, 3)"
      db.execute 'insert into conferences_t values (3, "confname")'

      organization = @organization_service.get 1

      expect( organization ).to_not be_nil
      expect( organization ).to     be_a   Organization

      expect( organization.conferences ).to_not be_nil
      expect( organization.conferences ).to     be_a   Array

      expect( organization.conferences.length ).to eq 1

      expect( organization.conferences[0].organization_id ).to eq 1
      expect( organization.conferences[0].conference_id   ).to eq 3

      expect( organization.conferences[0].conference ).to_not be_nil

      expect( organization.conferences[0].conference.conference_id ).to eq 3
      expect( organization.conferences[0].conference.name          ).to eq 'confname'
    end
  end

  describe '#save' do
    before :each do
      @mock_repository      = double( 'Repository' )
      conference_service    = ConferenceService.new @mock_repository, nil
      @organization_service = OrganizationService.new @mock_repository, conference_service
    end

    it 'should save the organization' do
      organization = Organization.build 1

      expect( @mock_repository ).to receive( :create ).with organization

      @organization_service.save organization
    end

    it 'should save the organization conference' do
      organization            = Organization.build 1
      organization_conference = OrganizationConference.build 1, 2

      organization.conferences = organization_conference

      expect( @mock_repository ).to receive( :create ).with organization
      expect( @mock_repository ).to receive( :create ).with organization_conference

      @organization_service.save organization
    end

    it 'should save the organization conference and conference ' do
      organization            = Organization.build 1
      organization_conference = OrganizationConference.build 1, 2
      conference              = Conference.build 2

      organization_conference.conference = conference
      organization.conferences           = organization_conference

      expect( @mock_repository ).to receive( :create ).with organization
      expect( @mock_repository ).to receive( :create ).with organization_conference
      expect( @mock_repository ).to receive( :create ).with conference

      @organization_service.save organization
    end

    it 'should save many organization conferences' do
      organization             = Organization.build 1
      organization_conference1 = OrganizationConference.build 1, 2
      organization_conference2 = OrganizationConference.build 1, 5

      organization.conferences = [organization_conference1, organization_conference2]

      expect( @mock_repository ).to receive( :create ).with organization
      expect( @mock_repository ).to receive( :create ).with organization_conference1
      expect( @mock_repository ).to receive( :create ).with organization_conference2

      @organization_service.save organization
    end
  end
end
